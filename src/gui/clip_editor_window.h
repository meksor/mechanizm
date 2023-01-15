
#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

#include "clip.h"
#include "player_widget.h"
#include "rythmic_point_table.h"

namespace mechanizm {

class ClipEditorWindow : public QMainWindow {
  Q_OBJECT
public:
  ClipEditorWindow(QWidget *parent = nullptr,
                   Qt::WindowFlags flags = Qt::WindowFlags());

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
  QAction *removeAct;
  QAction *addAct;
  QMenu *rpMenu;
  QMenu *playerMenu;
  mechanizm::RythmicPointTable *rpTable;
  mechanizm::PlayerWidget *player;
  mechanizm::Project *project;
};
} // namespace mechanizm