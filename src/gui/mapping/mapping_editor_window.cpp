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
#include <cmath>
#include <exception>
#include <map>
#include <memory>
#include <set>
#include <libopenshot/FFmpegWriter.h>
#include <libopenshot/FFmpegReader.h>

namespace mechanizm {

namespace {

struct PreviewAudioTrack {
  mechanizm::Source *source = nullptr;
  std::vector<double> positionsSeconds;
};

int wrapMappingPoint(int point, int size,
                     mechanizm::Mapping::WrapBehaviour wrapBehaviour) {
  if (size <= 0) {
    return 0;
  }

  if (wrapBehaviour == mechanizm::Mapping::WrapBehaviour::BOUNCE) {
    if (size <= 1) {
      return 0;
    }
    const int period = 2 * (size - 1);
    int mod = point % period;
    if (mod < 0) {
      mod += period;
    }
    return (mod < size) ? mod : period - mod;
  }

  int wrapped = point % size;
  if (wrapped < 0) {
    wrapped += size;
  }
  return wrapped;
}

double mappedSourceFrameAtOutputFrame(mechanizm::Mapping *mapping,
                                      double outputFrame, double bpm) {
  if (mapping == nullptr || mapping->clip == nullptr ||
      mapping->clip->source == nullptr ||
      mapping->clip->source->reader == nullptr) {
    return outputFrame;
  }

  const auto &rythmicPoints = mapping->clip->rythmicPoints;
  if (rythmicPoints.empty()) {
    return outputFrame;
  }

  if (bpm <= 0.0) {
    return rythmicPoints.front().frame;
  }

  openshot::Fraction fps = mapping->clip->source->reader->info.fps;
  const double noteLengthInFrames = (60.0 * fps.ToFloat()) / bpm;
  if (noteLengthInFrames <= 0.0) {
    return rythmicPoints.front().frame;
  }

  auto timeSeries = mapping->getChannelTimeseries();
  int point = 0;
  double sourceFrame = rythmicPoints.front().frame;
  for (const auto &step : timeSeries) {
    const auto &timeStep = std::get<0>(step);
    const auto *channel = std::get<1>(step);

    if (channel == nullptr) {
      continue;
    }

    const double eventFrame = std::round(noteLengthInFrames * timeStep.note);
    if (outputFrame < eventFrame) {
      break;
    }

    switch (channel->effect) {
    case mechanizm::Channel::Effect::INC:
      point += channel->value;
      break;
    case mechanizm::Channel::Effect::DEC:
      point -= channel->value;
      break;
    case mechanizm::Channel::Effect::SET:
      point = channel->value;
      break;
    }

    point = wrapMappingPoint(point, static_cast<int>(rythmicPoints.size()),
                             mapping->wrapBehaviour);
    sourceFrame = rythmicPoints[point].frame;
  }

  return sourceFrame;
}

std::vector<PreviewAudioTrack>
collectPreviewAudioTracks(mechanizm::Mapping *mapping, double bpm) {
  std::vector<PreviewAudioTrack> tracks;
  if (mapping == nullptr || bpm <= 0.0) {
    return tracks;
  }

  std::map<mechanizm::id_t, PreviewAudioTrack> trackMap;
  std::map<mechanizm::id_t, std::set<int64_t>> dedupedPositions;
  for (const auto &channel : mapping->channels) {
    auto *sequence = channel.sequence;
    if (sequence == nullptr || sequence->audioSource == nullptr ||
        sequence->audioSource->type != mechanizm::Source::Type::AUDIO) {
      continue;
    }

    auto &track = trackMap[sequence->audioSource->id];
    track.source = sequence->audioSource;
    auto &positions = dedupedPositions[sequence->audioSource->id];

    for (const auto &timeStep : sequence->timeSteps) {
      const double seconds = std::max(0.0, 60.0 * timeStep.note / bpm);
      const int64_t micros = static_cast<int64_t>(std::llround(seconds * 1000000.0));
      if (positions.insert(micros).second) {
        track.positionsSeconds.push_back(seconds);
      }
    }
  }

  tracks.reserve(trackMap.size());
  for (auto &[sourceId, track] : trackMap) {
    std::sort(track.positionsSeconds.begin(), track.positionsSeconds.end());
    tracks.push_back(track);
  }

  return tracks;
}

openshot::ReaderInfo buildPreviewInfo(openshot::Clip *clip,
                                      const std::vector<PreviewAudioTrack> &tracks) {
  auto info = clip->Reader()->info;
  if (tracks.empty()) {
    return info;
  }

  info.has_audio = true;
  if (info.sample_rate > 0 && info.channels > 0) {
    return info;
  }

  for (const auto &track : tracks) {
    if (track.source == nullptr || track.source->reader == nullptr) {
      continue;
    }

    if (!track.source->reader->IsOpen()) {
      track.source->reader->Open();
    }

    const auto &audioInfo = track.source->reader->info;
    if (audioInfo.sample_rate > 0 && audioInfo.channels > 0) {
      info.sample_rate = audioInfo.sample_rate;
      info.channels = audioInfo.channels;
      info.channel_layout = audioInfo.channel_layout;
      info.audio_bit_rate = audioInfo.audio_bit_rate;
      break;
    }
  }

  return info;
}

} // namespace

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

  overviewPanel = new QVBoxLayout();
  mappingTable = new mechanizm::MappingTable();
  mappingInfo = new mechanizm::MappingInfo();
  mappingInfo->setFixedWidth(250);
  overviewPanel->addWidget(mappingTable);
  overviewPanel->addWidget(mappingInfo);
  hbox->addLayout(overviewPanel);

  connect(mappingTable, &mechanizm::MappingTable::selectMapping, this,
          &MappingEditorWindow::onMappingSelected);
  connect(mappingTable, &mechanizm::MappingTable::selectMapping, mappingInfo,
          &MappingInfo::onMappingSelected);

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
  wrapBehaviourEdit->addItem(
      "Loop", static_cast<int>(mechanizm::Mapping::WrapBehaviour::LOOP));
  wrapBehaviourEdit->addItem(
      "Bounce", static_cast<int>(mechanizm::Mapping::WrapBehaviour::BOUNCE));
  mappingSettingsLayout->addRow(tr("Wrap"), wrapBehaviourEdit);
  leftPanel->addWidget(mappingSettingsWidget);
  connect(wrapBehaviourEdit, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, [this](int index) {
            onWrapBehaviourChanged(wrapBehaviourEdit->itemData(index).toInt());
          });

  auto *channelEditorWidget = new QWidget();
  auto *channelEditorLayout = new QFormLayout(channelEditorWidget);
  effectEdit = new QComboBox(channelEditorWidget);
  effectEdit->addItem("Inc", static_cast<int>(mechanizm::Channel::Effect::INC));
  effectEdit->addItem("Dec", static_cast<int>(mechanizm::Channel::Effect::DEC));
  effectEdit->addItem("Set", static_cast<int>(mechanizm::Channel::Effect::SET));

  interpolationEdit = new QComboBox(channelEditorWidget);
  interpolationEdit->addItem(
      "None", static_cast<int>(mechanizm::Channel::Interpolation::NONE));
  interpolationEdit->addItem(
      "Linear", static_cast<int>(mechanizm::Channel::Interpolation::LINEAR));

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
  connect(interpolationEdit, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, [this](int index) {
            onChannelInterpolationChanged(
                interpolationEdit->itemData(index).toInt());
          });
  connect(valueEdit, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MappingEditorWindow::onChannelValueChanged);

  hbox->addLayout(leftPanel);

  rightPanel = new QVBoxLayout();

  clipTimeline = new mechanizm::ClipTimeline();
  clipTimeline->setFixedHeight(130);
  rightPanel->addWidget(clipTimeline);

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

void MappingEditorWindow::onProjectChanged(mechanizm::Project *p) {
  project = p;
  connect(project, &Project::mappingsChanged, mappingTable,
          &mechanizm::MappingTable::onMappingsChanged,
          Qt::UniqueConnection);
  if (timeline != nullptr && project != nullptr) {
    timeline->setBpm(project->bpm);
  }
  renderPreview();
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
  mappingInfo->onMappingSelected(mapping);
  selectedChannelRow = -1;
  chTable->onMappingUpdated(mapping);
  timeline->onMappingSelected(mapping);
  clipTimeline->onClipSelected(mapping != nullptr ? mapping->clip : nullptr);
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

  if (player != nullptr && player->player != nullptr) {
    player->player->Pause();
  }

  openshot::Clip *clip = this->project->compositor.compose(mapping);
  if (clip == nullptr) {
    return;
  }

  if (!clip->Reader()->IsOpen()) {
    clip->Reader()->Open();
  }
  const auto previewAudioTracks =
      collectPreviewAudioTracks(mapping, project->bpm);
  auto info = buildPreviewInfo(clip, previewAudioTracks);

  const bool recreateTimeline =
      previewTimeline == nullptr ||
      previewTimeline->info.width != info.width ||
      previewTimeline->info.height != info.height ||
      previewTimeline->info.sample_rate != info.sample_rate ||
      previewTimeline->info.channels != info.channels ||
      previewTimeline->info.channel_layout != info.channel_layout ||
      previewTimeline->info.fps.num != info.fps.num ||
      previewTimeline->info.fps.den != info.fps.den;

  if (recreateTimeline) {
    if (previewTimeline != nullptr) {
      delete previewTimeline;
      previewTimeline = nullptr;
    }
    previewTimeline = new openshot::Timeline(info);
    if (previewTimeline->GetCache() != nullptr) {
      previewTimeline->GetCache()->SetMaxBytesFromInfo(
          120, info.width, info.height,
          info.sample_rate, info.channels);
    }
  } else {
    previewTimeline->Clear();
  }

  previewAudioClips.clear();
  previewTimeline->AddClip(clip);
  previewClip = clip;

  int audioLayer = 1;
  for (const auto &track : previewAudioTracks) {
    if (track.source == nullptr || track.source->reader == nullptr) {
      continue;
    }

    if (!track.source->reader->IsOpen()) {
      track.source->reader->Open();
    }

    const double duration =
        std::max(0.05, static_cast<double>(track.source->reader->info.duration));
    for (double positionSeconds : track.positionsSeconds) {
      auto audioClip = std::make_unique<openshot::Clip>(track.source->path);
      audioClip->Position(static_cast<float>(positionSeconds));
      audioClip->Start(0.0f);
      audioClip->End(static_cast<float>(duration));
      audioClip->Layer(audioLayer);
      audioClip->alpha.AddPoint(1, 0.0);
      previewTimeline->AddClip(audioClip.get());
      previewAudioClips.push_back(std::move(audioClip));
    }
    audioLayer++;
  }

  previewTimeline->ClearAllCache(false);
  player->setReader(previewTimeline);

  if (clip->GetCache() != nullptr) {
    clip->GetCache()->SetMaxBytesFromInfo(
        120, info.width, info.height,
        info.sample_rate, info.channels);
  }

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
  if (player == nullptr || player->player == nullptr) {
    return;
  }

  const double outputFrame = player->player->Position();
  if (timeline != nullptr) {
    timeline->setCursorFrame(outputFrame);
  }
  if (clipTimeline != nullptr) {
    const double sourceFrame = mappedSourceFrameAtOutputFrame(
        mapping, outputFrame, project != nullptr ? project->bpm : 120.0);
    clipTimeline->setCursorFrame(sourceFrame);
  }
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
