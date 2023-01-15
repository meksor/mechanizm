#include "gui/clips_window.h"
#include "clip.h"
#include "gui/clip_info.h"
#include "gui/clip_table.h"
#include "json.h"
#include "project.h"
#include <QAction>
#include <QDebug>
#include <QFileDialog>

namespace mechanizm {

ClipsWindow::ClipsWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Clips");
  createActions();
  createMenus();
  QWidget *widget = new QWidget;

  hbox = new QHBoxLayout(widget);
  clipTable = new mechanizm::ClipTable();
  hbox->addWidget(clipTable);

  clipInfo = new mechanizm::ClipInfo();
  clipInfo->setFixedWidth(250);
  hbox->addWidget(clipInfo);

  setCentralWidget(widget);
  this->setFixedSize(550, 300);

  connect(clipTable, &ClipTable::selectClip, clipInfo,
          &ClipInfo::onClipSelected);
}

void ClipsWindow::createActions() {
  removeAct = new QAction(tr("&Remove Clip"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected clip"));

  connect(removeAct, &QAction::triggered, this,
          &ClipsWindow::removeSelectedClip);
}

void ClipsWindow::createMenus() {
  clipMenu = this->menuBar()->addMenu(tr("&Clip"));
  clipMenu->addAction(removeAct);
}

void ClipsWindow::removeSelectedClip() {
  project->removeClip(clipTable->getSelectedClip());
}

} // namespace mechanizm