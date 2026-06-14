
#pragma once

#include "gui/clip/clip_info.h"
#include "gui/clip/clip_table.h"
#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "clip.h"
#include "gui/mapping/clip_timeline.h"
#include "gui/player_widget.h"
#include "rythmic_point_table.h"

namespace mechanizm {

class ClipEditorWindow : public QMainWindow {
  Q_OBJECT
public:
  ClipEditorWindow(QWidget *parent = nullptr,
                   Qt::WindowFlags flags = Qt::WindowFlags());

  void onProjectChanged(mechanizm::Project *p);
  void onClipSelected(mechanizm::Clip *);

  void createMenus();
  void createActions();

  void removeSelectedRythmicPoint();
  void addRythmicPoint();

protected:
private:
  void updateTable();
  // void addRythmicPoint();

  QHBoxLayout *hbox;
  QVBoxLayout *leftColumn;
  QVBoxLayout *previewColumn;
  QAction *removeAct;
  QAction *addAct;
  QMenu *rpMenu;
  QMenu *playerMenu;
  mechanizm::ClipTable *clipTable;
  mechanizm::ClipInfo *clipInfo;
  mechanizm::RythmicPointTable *rpTable;
  mechanizm::PlayerWidget *player;
  mechanizm::ClipTimeline *timeline;
  mechanizm::Project *project = nullptr;
  mechanizm::Clip *clip = nullptr;
  QTimer *cursorSyncTimer = nullptr;
};
} // namespace mechanizm