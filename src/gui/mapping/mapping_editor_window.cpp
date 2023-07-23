#include "mapping_editor_window.h"
#include "clip.h"
#include "mapping.h"
#include <QHeaderView>

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
  removeAct = new QAction(tr("&Remove Channel"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected channel"));

  addAct = new QAction(tr("&Add Channel"), this);
  addAct->setShortcut(Qt::Key_C);
  addAct->setStatusTip(tr("Add a new channel"));

  connect(removeAct, &QAction::triggered, this,
          &MappingEditorWindow::removeSelectedChannel);
  connect(addAct, &QAction::triggered, this, &MappingEditorWindow::addChannel);
}

void MappingEditorWindow::createMenus() {
  chMenu = this->menuBar()->addMenu(tr("&Channels"));
  chMenu->addAction(removeAct);
  chMenu->addAction(addAct);
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
} // namespace mechanizm
