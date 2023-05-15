#pragma once

#include "gui/mapping/mapping_info.h"
#include "gui/mapping/mapping_table.h"
#include "project.h"
#include "mapping.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <vector>

namespace mechanizm {

class MappingsWindow : public QMainWindow {
  Q_OBJECT
public:
  MappingsWindow(QWidget *parent = nullptr,
                  Qt::WindowFlags flags = Qt::WindowFlags());

  void createMenus();
  void createActions();

  void removeSelectedMapping();

  void onProjectChanged(mechanizm::Project *p) {
    project = p;
    connect(project, &Project::mappingsChanged, mappingTable,
            &mechanizm::MappingTable::onMappingsChanged);
  };

private:
  mechanizm::MappingTable *mappingTable;
  mechanizm::MappingInfo *mappingInfo;
  mechanizm::Project *project;

  QHBoxLayout *hbox;
  QAction *removeAct;
  QMenu *mappingMenu;
};
} // namespace mechanizm