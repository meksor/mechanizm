#include "mapping_editor_window.h"
#include "FFmpegReader.h"
#include "Fraction.h"
#include "FrameMapper.h"
#include "clip.h"
#include "mapping.h"
#include "mapping_timeline.h"
#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QStatusBar>
#include <QStandardPaths>
#include <QVBoxLayout>
#include <algorithm>
#include <exception>
#include <libopenshot/FFmpegWriter.h>
#include <libopenshot/FFmpegReader.h>

namespace mechanizm {

static QString sanitizeFileComponent(QString text) {
  text = text.trimmed();
  if (text.isEmpty()) {
    return "mapping";
  }
  for (QChar &ch : text) {
    if (!(ch.isLetterOrNumber() || ch == '_' || ch == '-')) {
      ch = '_';
    }
  }
  return text;
}

MappingEditorWindow::MappingEditorWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Mapping Editor");
  createActions();

  QWidget *widget = new QWidget;
  hbox = new QHBoxLayout(widget);

    leftPanel = new QVBoxLayout();

  chTable = new mechanizm::ChannelTable();
  chTable->setFixedWidth(250);
    leftPanel->addWidget(chTable);
  connect(chTable, &mechanizm::ChannelTable::selectChannel, this,
          &MappingEditorWindow::onChannelSelected);

    // Mapping-level settings
    auto *mappingSettingsWidget = new QWidget();
    auto *mappingSettingsLayout = new QFormLayout(mappingSettingsWidget);
    wrapBehaviourEdit = new QComboBox(mappingSettingsWidget);
    wrapBehaviourEdit->addItem("Loop", static_cast<int>(mechanizm::Mapping::WrapBehaviour::LOOP));
    wrapBehaviourEdit->addItem("Bounce", static_cast<int>(mechanizm::Mapping::WrapBehaviour::BOUNCE));
    mappingSettingsLayout->addRow(tr("Wrap"), wrapBehaviourEdit);
    leftPanel->addWidget(mappingSettingsWidget);
    connect(wrapBehaviourEdit, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      [this](int index) {
        onWrapBehaviourChanged(wrapBehaviourEdit->itemData(index).toInt());
      });

    auto *channelEditorWidget = new QWidget();
    auto *channelEditorLayout = new QFormLayout(channelEditorWidget);
    effectEdit = new QComboBox(channelEditorWidget);
    effectEdit->addItem("Inc", static_cast<int>(mechanizm::Channel::Effect::INC));
    effectEdit->addItem("Dec", static_cast<int>(mechanizm::Channel::Effect::DEC));
    effectEdit->addItem("Set", static_cast<int>(mechanizm::Channel::Effect::SET));

    interpolationEdit = new QComboBox(channelEditorWidget);
    interpolationEdit->addItem("None",
             static_cast<int>(mechanizm::Channel::Interpolation::NONE));
    interpolationEdit->addItem("Linear", static_cast<int>(
              mechanizm::Channel::Interpolation::LINEAR));

    valueEdit = new QSpinBox(channelEditorWidget);
    valueEdit->setRange(-9999, 9999);

    channelEditorLayout->addRow(tr("Effect"), effectEdit);
    channelEditorLayout->addRow(tr("Interpolation"), interpolationEdit);
    channelEditorLayout->addRow(tr("Value"), valueEdit);
    leftPanel->addWidget(channelEditorWidget);
    leftPanel->addStretch();

    connect(effectEdit, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      [this](int index) {
        onChannelEffectChanged(effectEdit->itemData(index).toInt());
      });
    connect(interpolationEdit,
      QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      [this](int index) {
        onChannelInterpolationChanged(
      interpolationEdit->itemData(index).toInt());
      });
    connect(valueEdit, QOverload<int>::of(&QSpinBox::valueChanged), this,
      &MappingEditorWindow::onChannelValueChanged);

    hbox->addLayout(leftPanel);

  rightPanel = new QVBoxLayout();

  timeline = new mechanizm::MappingTimeline();
  timeline->setFixedHeight(180);
  rightPanel->addWidget(timeline);

  player = new mechanizm::PlayerWidget();
  player->setFixedSize(720, 480);
  player->renderWidget->setFixedSize(720, 480);

  rightPanel->addWidget(player);
  hbox->addLayout(rightPanel);

    previewRefreshTimer = new QTimer(this);
    previewRefreshTimer->setSingleShot(true);
    previewRefreshTimer->setInterval(120);
    connect(previewRefreshTimer, &QTimer::timeout, this,
      &MappingEditorWindow::renderPreview);

    cursorSyncTimer = new QTimer(this);
    cursorSyncTimer->setInterval(33);
    connect(cursorSyncTimer, &QTimer::timeout, this,
      &MappingEditorWindow::updateTimelineCursor);
    cursorSyncTimer->start();

    refreshChannelEditor();

  createMenus();

  renderStatusLabel = new QLabel(tr("Render: idle"), this);
  renderProgressBar = new QProgressBar(this);
  renderProgressBar->setMinimum(0);
  renderProgressBar->setMaximum(100);
  renderProgressBar->setFixedWidth(180);
  statusBar()->addWidget(renderStatusLabel, 1);
  statusBar()->addPermanentWidget(renderProgressBar);
  updateRenderStatusUi(false, 0, tr("Render: idle"));

  setCentralWidget(widget);
}

void MappingEditorWindow::createActions() {
  previewAct = new QAction(tr("&Render Preview"), this);
  previewAct->setShortcut(Qt::Key_R);
  previewAct->setStatusTip(tr("Render a preview for this mapping"));

  removeAct = new QAction(tr("&Remove Channel"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected channel"));

  addAct = new QAction(tr("&Add Channel"), this);
  addAct->setShortcut(Qt::Key_C);
  addAct->setStatusTip(tr("Add a new channel"));

  connect(previewAct, &QAction::triggered, this,
      &MappingEditorWindow::onRenderPreviewRequested);
  connect(removeAct, &QAction::triggered, this,
          &MappingEditorWindow::removeSelectedChannel);
  connect(addAct, &QAction::triggered, this, &MappingEditorWindow::addChannel);
}

void MappingEditorWindow::createMenus() {
  chMenu = this->menuBar()->addMenu(tr("&Channels"));
  chMenu->addAction(removeAct);
  chMenu->addAction(addAct);
  compMenu = this->menuBar()->addMenu(tr("&Compositor"));
  compMenu->addAction(previewAct);

  playerMenu = this->menuBar()->addMenu(tr("&Player"));
  playerMenu->addAction(player->iSpdAct);
  playerMenu->addAction(player->dSpdAct);
  playerMenu->addAction(player->tpAct);
  playerMenu->addAction(player->tsAct);
  playerMenu->addSeparator();
  playerMenu->addAction(player->iFrmAct);
  playerMenu->addAction(player->dFrmAct);
}
void MappingEditorWindow::onChannelSelected(mechanizm::Channel *c) {
  (void)c;
  selectedChannelRow = chTable->currentRow();
  refreshChannelEditor();
}
void MappingEditorWindow::removeSelectedChannel() {
  if (mapping == nullptr) {
    return;
  }

  const int row = chTable->currentRow();
  if (row < 0 || row >= static_cast<int>(mapping->channels.size())) {
    return;
  }

  mechanizm::Channel ch = chTable->getSelectedChannel();
  mapping->removeChannel(ch);
};
void MappingEditorWindow::addChannel() {
  if (mapping == nullptr || sequence == nullptr) {
    return;
  }
  mechanizm::Channel ch(sequence, mechanizm::Channel::Effect::INC,
                        mechanizm::Channel::Interpolation::LINEAR, 1);
  mapping->addChannel(ch);
};

void MappingEditorWindow::onMappingSelected(mechanizm::Mapping *m) {
  disconnectMappingSignals();
  mapping = m;
  selectedChannelRow = -1;
  chTable->onMappingUpdated(mapping);
  timeline->onMappingSelected(mapping);
  refreshChannelEditor();

  if (mapping != nullptr) {
    connect(mapping, &Mapping::updated, this, [this]() {
      chTable->onMappingUpdated(mapping);
      if (mapping == nullptr || mapping->channels.empty()) {
        selectedChannelRow = -1;
      } else if (selectedChannelRow >=
                 static_cast<int>(mapping->channels.size())) {
        selectedChannelRow = static_cast<int>(mapping->channels.size()) - 1;
      }
      refreshChannelEditor();
      schedulePreviewRender();
    });
  }

  renderPreview();
}

void MappingEditorWindow::schedulePreviewRender() {
  if (previewRefreshTimer == nullptr) {
    return;
  }
  previewRefreshTimer->start();
}

void MappingEditorWindow::renderPreview() {
  if (project == nullptr || mapping == nullptr || mapping->clip == nullptr) {
    return;
  }

  player->player->Stop();

  if (previewTimeline != nullptr) {
    delete previewTimeline;
    previewTimeline = nullptr;
  }

  openshot::Clip *clip = this->project->compositor.compose(mapping);
  if (clip == nullptr) {
    return;
  }

  if (!clip->Reader()->IsOpen()) {
    clip->Reader()->Open();
  }
  auto info = clip->Reader()->info;
  previewTimeline = new openshot::Timeline(info);
  previewTimeline->AddClip(clip);
  player->setReader(previewTimeline);
  updateTimelineCursor();
}

QString MappingEditorWindow::defaultPreviewPath() const {
  QString baseDir = previewOutputPath;
  if (!baseDir.isEmpty()) {
    QFileInfo existing(baseDir);
    baseDir = existing.absolutePath();
  }

  if (baseDir.isEmpty()) {
    baseDir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
  }
  if (baseDir.isEmpty()) {
    baseDir = QDir::homePath();
  }

  QString mappingName = "mapping";
  if (mapping != nullptr) {
    mappingName = QString::fromStdString(mapping->name);
  }
  QString clipName = "clip";
  if (mapping != nullptr && mapping->clip != nullptr) {
    clipName = QString::fromStdString(mapping->clip->name);
  }

  const QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
  const QString fileName =
      QString("preview_%1_%2_%3.mp4")
          .arg(sanitizeFileComponent(mappingName), sanitizeFileComponent(clipName), timestamp);
  return QDir(baseDir).filePath(fileName);
}

bool MappingEditorWindow::renderPreviewToFile(const QString &filePath) {
  if (previewTimeline == nullptr) {
    return false;
  }

  if (!previewTimeline->IsOpen()) {
    previewTimeline->Open();
  }

  const auto info = previewTimeline->info;
  if (info.video_length <= 0) {
    return false;
  }

  openshot::FFmpegWriter writer(filePath.toStdString());
  writer.SetVideoOptions(true, "libx264", info.fps, info.width, info.height,
                         info.pixel_ratio, false, false,
                         (info.video_bit_rate > 0 ? info.video_bit_rate : 8000000));

  if (info.has_audio) {
    writer.SetAudioOptions(true, "aac", info.sample_rate, info.channels,
                           info.channel_layout,
                           (info.audio_bit_rate > 0 ? info.audio_bit_rate : 192000));
  }

  writer.Open();
  const int64_t totalFrames = info.video_length;
  const int64_t updateEvery = std::max<int64_t>(1, totalFrames / 100);
  for (int64_t frame = 1; frame <= totalFrames; ++frame) {
    writer.WriteFrame(previewTimeline->GetFrame(frame));

    if (frame == 1 || frame == totalFrames || (frame % updateEvery) == 0) {
      const int progress = static_cast<int>((frame * 100) / totalFrames);
      updateRenderStatusUi(true, progress,
                           tr("Render: running (%1%)").arg(progress));
      QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
  }
  writer.Close();
  return true;
}

void MappingEditorWindow::onRenderPreviewRequested() {
  if (isRenderingPreview) {
    return;
  }

  renderPreview();

  if (previewTimeline == nullptr) {
    QMessageBox::warning(this, tr("Render Preview"),
                         tr("No preview is available to render."));
    return;
  }

  const QString startPath =
      previewOutputPath.isEmpty() ? defaultPreviewPath() : previewOutputPath;
  const QString targetPath = QFileDialog::getSaveFileName(
      this, tr("Render Preview To File"), startPath,
      tr("MP4 Video (*.mp4);;MOV Video (*.mov);;MKV Video (*.mkv);;All Files (*)"));

  if (targetPath.isEmpty()) {
    return;
  }

  previewOutputPath = targetPath;

  isRenderingPreview = true;
  previewAct->setEnabled(false);
  updateRenderStatusUi(true, 0, tr("Render: running (0%)"));

  try {
    if (!renderPreviewToFile(targetPath)) {
      isRenderingPreview = false;
      previewAct->setEnabled(true);
      updateRenderStatusUi(false, 0, tr("Render: failed"));
      QMessageBox::warning(this, tr("Render Preview"),
                           tr("Preview rendering failed."));
      return;
    }
  } catch (const std::exception &e) {
    isRenderingPreview = false;
    previewAct->setEnabled(true);
    updateRenderStatusUi(false, 0, tr("Render: failed"));
    QMessageBox::critical(this, tr("Render Preview"),
                          tr("Rendering failed: %1").arg(e.what()));
    return;
  }

  isRenderingPreview = false;
  previewAct->setEnabled(true);
  updateRenderStatusUi(false, 100, tr("Render: done"));

  QMessageBox::information(this, tr("Render Preview"),
                           tr("Preview rendered to:\n%1").arg(targetPath));
}

void MappingEditorWindow::updateRenderStatusUi(bool running, int progress,
                                               const QString &message) {
  if (renderStatusLabel != nullptr) {
    renderStatusLabel->setText(message);
  }
  if (renderProgressBar != nullptr) {
    renderProgressBar->setValue(progress);
    renderProgressBar->setVisible(running || progress > 0);
  }
}

void MappingEditorWindow::updateTimelineCursor() {
  if (timeline == nullptr || player == nullptr || player->player == nullptr) {
    return;
  }

  timeline->setCursorFrame(player->player->Position());
}

void MappingEditorWindow::refreshChannelEditor() {
  const bool hasChannel =
      (mapping != nullptr && selectedChannelRow >= 0 &&
       selectedChannelRow < static_cast<int>(mapping->channels.size()));
  effectEdit->setEnabled(hasChannel);
  interpolationEdit->setEnabled(hasChannel);
  valueEdit->setEnabled(hasChannel);

  wrapBehaviourEdit->setEnabled(mapping != nullptr);
  if (mapping != nullptr) {
    const QSignalBlocker wrapBlocker(wrapBehaviourEdit);
    const int wrapIndex = wrapBehaviourEdit->findData(static_cast<int>(mapping->wrapBehaviour));
    if (wrapIndex >= 0) {
      wrapBehaviourEdit->setCurrentIndex(wrapIndex);
    }
  }

  if (!hasChannel) {
    return;
  }

  const QSignalBlocker effectBlocker(effectEdit);
  const QSignalBlocker interpolationBlocker(interpolationEdit);
  const QSignalBlocker valueBlocker(valueEdit);

  const mechanizm::Channel &currentChannel = mapping->channels[selectedChannelRow];

  const int effectIndex =
      effectEdit->findData(static_cast<int>(currentChannel.effect));
  if (effectIndex >= 0) {
    effectEdit->setCurrentIndex(effectIndex);
  }

  const int interpolationIndex =
      interpolationEdit->findData(static_cast<int>(currentChannel.interpolation));
  if (interpolationIndex >= 0) {
    interpolationEdit->setCurrentIndex(interpolationIndex);
  }

  valueEdit->setValue(currentChannel.value);
}

void MappingEditorWindow::onChannelEffectChanged(int effect) {
  if (mapping == nullptr || selectedChannelRow < 0 ||
      selectedChannelRow >= static_cast<int>(mapping->channels.size())) {
    return;
  }
  mapping->channels[selectedChannelRow].effect =
      static_cast<mechanizm::Channel::Effect>(effect);
  mapping->onChannelsChanged();
}

void MappingEditorWindow::onChannelInterpolationChanged(int interpolation) {
  if (mapping == nullptr || selectedChannelRow < 0 ||
      selectedChannelRow >= static_cast<int>(mapping->channels.size())) {
    return;
  }
  mapping->channels[selectedChannelRow].interpolation =
      static_cast<mechanizm::Channel::Interpolation>(interpolation);
  mapping->onChannelsChanged();
}

void MappingEditorWindow::onChannelValueChanged(int value) {
  if (mapping == nullptr || selectedChannelRow < 0 ||
      selectedChannelRow >= static_cast<int>(mapping->channels.size())) {
    return;
  }
  mapping->channels[selectedChannelRow].value = value;
  mapping->onChannelsChanged();
}

void MappingEditorWindow::onWrapBehaviourChanged(int wrapBehaviour) {
  if (mapping == nullptr) {
    return;
  }
  mapping->wrapBehaviour = static_cast<mechanizm::Mapping::WrapBehaviour>(wrapBehaviour);
  schedulePreviewRender();
}

void MappingEditorWindow::disconnectMappingSignals() {
  if (mapping != nullptr) {
    disconnect(mapping, &Mapping::updated, this, nullptr);
  }
}

} // namespace mechanizm
