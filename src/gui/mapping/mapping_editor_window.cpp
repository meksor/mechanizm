#include "mapping_editor_window.h"
#include "FFmpegReader.h"
#include "Fraction.h"
#include "FrameMapper.h"
#include "clip.h"
#include "mapping.h"
#include <QHeaderView>
#include <cmath>
#include <libopenshot/FFmpegReader.h>
#include <libopenshot/FFmpegWriter.h>

namespace mechanizm {
MappingEditorWindow::MappingEditorWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Mapping Editor");
  createActions();

  QWidget *widget = new QWidget;
  hbox = new QHBoxLayout(widget);

  chTable = new mechanizm::ChannelTable();
  chTable->setFixedWidth(250);
  hbox->addWidget(chTable);
  connect(chTable, &mechanizm::ChannelTable::selectChannel, this,
          &MappingEditorWindow::onChannelSelected);

  player = new mechanizm::PlayerWidget();
  player->setFixedSize(720, 480);
  player->renderWidget->setFixedSize(720, 480);

  hbox->addWidget(player);
  createMenus();
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
          &MappingEditorWindow::renderPreview);
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
  channel = c;
  // channelInfo->onChannelSelected(clip);
}
void MappingEditorWindow::removeSelectedChannel() {
  mechanizm::Channel ch = chTable->getSelectedChannel();
  mapping->removeChannel(ch);
  chTable->onMappingUpdated(mapping);
};
void MappingEditorWindow::addChannel() {
  mechanizm::Channel ch(sequence, mechanizm::Channel::Effect::INC,
                        mechanizm::Channel::Interpolation::LINEAR, 1);
  mapping->addChannel(ch);
  chTable->onMappingUpdated(mapping);
};

void MappingEditorWindow::onMappingSelected(mechanizm::Mapping *m) {
  mapping = m;
  chTable->onMappingUpdated(mapping);
  // channelInfo->onChannelSelected(clip);
}

void MappingEditorWindow::renderPreview() {
  if (previewTimeline != nullptr) {
    delete previewTimeline;
  }
  openshot::Clip *clip = this->project->compositor.compose(mapping);
  auto info = clip->Reader()->info;
  previewTimeline = new openshot::Timeline(info);
  previewTimeline->AddClip(clip);
  player->setReader(previewTimeline);

  openshot::Timeline renderTimeline(info);
  renderTimeline.AddClip(clip);

  renderTimeline.Open();
  openshot::FFmpegWriter writer("./test.mkv");
  writer.SetVideoOptions(true, "libx265", info.fps, info.width, info.height,
                         info.pixel_ratio, false, false, 300000);

  writer.Open();
  int length = renderTimeline.GetMaxFrame();
  writer.WriteFrame(&renderTimeline, 0, length);
  writer.Close();
  renderTimeline.Close();
}

} // namespace mechanizm
