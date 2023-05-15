#include "gui/mappings_window.h"
#include "clip.h"
#include "json.h"
#include "project.h"
#include "source.h"
#include <QAction>
#include <QDebug>
#include <QMenuBar>

namespace mechanizm {

MappingsWindow::MappingsWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Mappings");
  createActions();
  createMenus();

  QWidget *widget = new QWidget;
  hbox = new QHBoxLayout(widget);

  mappingTable = new mechanizm::MappingTable();
  hbox->addWidget(mappingTable);

  mappingInfo = new mechanizm::MappingInfo();
  mappingInfo->setFixedWidth(200);
  hbox->addWidget(mappingInfo);

  setCentralWidget(widget);
  this->setFixedSize(700, 300);

  connect(mappingTable, &MappingTable::selectMapping, mappingInfo,
          &MappingInfo::onMappingSelected);
}

void MappingsWindow::createActions() {
  removeAct = new QAction(tr("&Remove Mapping"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected mapping"));

  connect(removeAct, &QAction::triggered, this,
          &MappingsWindow::removeSelectedMapping);
}

void MappingsWindow::createMenus() {
  mappingMenu = this->menuBar()->addMenu(tr("&Mapping"));
  mappingMenu->addAction(removeAct);
}

void MappingsWindow::removeSelectedMapping() {
  project->removeMapping(mappingTable->getSelectedMapping());
}

} // namespace mechanizm