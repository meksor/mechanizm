#pragma once

#include "gui/clip/clip_info.h"
#include "gui/clip/clip_table.h"
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
  void mapSelectedClip();
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
  QAction *mapAct;
  QMenu *clipMenu;
};
} // namespace mechanizm