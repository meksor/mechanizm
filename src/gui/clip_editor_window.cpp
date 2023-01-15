#include "clip_editor_window.h"
#include <QHeaderView>

namespace mechanizm {
ClipEditorWindow::ClipEditorWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Clip Editor");
  createActions();

  QWidget *widget = new QWidget;
  hbox = new QHBoxLayout(widget);

  rpTable = new mechanizm::RythmicPointTable();
  rpTable->setFixedWidth(200);
  hbox->addWidget(rpTable);

  player = new mechanizm::PlayerWidget();
  player->setFixedSize(720, 480);
  player->renderWidget->setFixedSize(720, 480);

  hbox->addWidget(player);
  createMenus();

  setCentralWidget(widget);
}

void ClipEditorWindow::createActions() {
  removeAct = new QAction(tr("&Remove Rythmic Point"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected rythmic point"));

  addAct = new QAction(tr("&Add Rythmic Point"), this);
  addAct->setShortcut(Qt::Key_R);
  addAct->setStatusTip(tr("Add a new rythmic point"));

  connect(removeAct, &QAction::triggered, this,
          &ClipEditorWindow::removeSelectedRythmicPoint);
  connect(addAct, &QAction::triggered, this,
          &ClipEditorWindow::addRythmicPoint);
}

void ClipEditorWindow::createMenus() {
  rpMenu = this->menuBar()->addMenu(tr("&Rythmic Points"));
  rpMenu->addAction(removeAct);
  playerMenu = this->menuBar()->addMenu(tr("&Player"));
  playerMenu->addAction(player->iSpdAct);
  playerMenu->addAction(player->dSpdAct);
  playerMenu->addAction(player->tpAct);
  playerMenu->addAction(player->tsAct);
  playerMenu->addSeparator();
  playerMenu->addAction(player->iFrmAct);
  playerMenu->addAction(player->dFrmAct);
}
void ClipEditorWindow::onClipSelected(mechanizm::Clip *c) {
  player->setFilePath(c->source->path);
  rpTable->onClipUpdated(c);
}
void ClipEditorWindow::removeSelectedRythmicPoint(){

};
void ClipEditorWindow::addRythmicPoint(){

};

} // namespace mechanizm