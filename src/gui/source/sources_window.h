#pragma once

#include "gui/source/source_info.h"
#include "gui/source/source_table.h"
#include "project.h"
#include "source.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <vector>

namespace mechanizm {

class SourcesWindow : public QMainWindow {
  Q_OBJECT
public:
  static const QStringList nameFilters;

  SourcesWindow(QWidget *parent = nullptr,
                Qt::WindowFlags flags = Qt::WindowFlags());

  void createMenus();
  void createActions();

  void importSource();
  void removeSelectedSource();
  void convertSelectedSource();
  void onProjectChanged(mechanizm::Project *p) {
    project = p;
    connect(project, &Project::sourcesChanged, sourceTable,
            &mechanizm::SourceTable::onSourcesChanged);
  };

private:
  mechanizm::SourceTable *sourceTable;
  mechanizm::SourceInfo *sourceInfo;
  mechanizm::Project *project;

  QHBoxLayout *hbox;
  QAction *importAct;
  QAction *removeAct;
  QAction *convertAct;
  QMenu *sourceMenu;
};
const QStringList getNameFilters();
} // namespace mechanizm