#include "clip.h"
#include "gui/clip/clips_window.h"
#include "gui/clip/clip_info.h"
#include "gui/clip/clip_table.h"
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

  mapAct = new QAction(tr("&Map Clip"), this);
  mapAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
  mapAct->setStatusTip(tr("Map the selected clip"));

  connect(mapAct, &QAction::triggered, this,
          &ClipsWindow::mapSelectedClip);

}
 
void ClipsWindow::createMenus() {
  clipMenu = this->menuBar()->addMenu(tr("&Clip"));
  clipMenu->addAction(removeAct);
  clipMenu->addAction(mapAct);
}

void ClipsWindow::removeSelectedClip() {
  project->removeClip(clipTable->getSelectedClip());
}

void ClipsWindow::mapSelectedClip() {
  mechanizm::id_t id = mechanizm::Mapping::getNextId(project->mappings);
  mechanizm::Clip *clip = clipTable->getSelectedClip(); 
  mechanizm::Mapping *mapping =
        new mechanizm::Mapping(id, clip->name, clip);
      
  project->addMapping(mapping);
}

} // namespace mechanizm