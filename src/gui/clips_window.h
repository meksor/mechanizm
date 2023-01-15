#pragma once

#include "gui/clip_info.h"
#include "gui/clip_table.h"
#include "project.h"
#include "source.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <vector>

namespace mechanizm {

class ClipsWindow : public QMainWindow {
  Q_OBJECT
public:
  ClipsWindow(QWidget *parent = nullptr,
              Qt::WindowFlags flags = Qt::WindowFlags());

  void createMenus();
  void createActions();

  void removeSelectedClip();
  void onProjectChanged(mechanizm::Project *p) {
    project = p;
    connect(project, &Project::clipsChanged, clipTable,
            &mechanizm::ClipTable::onClipsChanged);
  };

  mechanizm::ClipTable *clipTable;
  mechanizm::ClipInfo *clipInfo;
  mechanizm::Project *project;

private:
  QHBoxLayout *hbox;
  QAction *removeAct;
  QMenu *clipMenu;
};
} // namespace mechanizm