#pragma once

#include "gui/clip_editor_window.h"
#include "gui/clips_window.h"
#include "gui/project_widget.h"
#include "gui/sequences_window.h"
#include "gui/sources_window.h"
#include "project.h"
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace mechanizm {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr,
             Qt::WindowFlags flags = Qt::WindowFlags());

  void createMenus();
  void createActions();

  void saveProject();
  void newProject();
  void openProject();
  void changeProject(mechanizm::Project *p);
signals:
  void projectChanged(mechanizm::Project *project);

private:
  mechanizm::Project *project = nullptr;
  mechanizm::ProjectWidget *projectWidget;
  mechanizm::SourcesWindow *sourcesWindow;
  mechanizm::ClipsWindow *clipsWindow;
  mechanizm::ClipEditorWindow *clipEditorWindow;
  mechanizm::SequencesWindow *sequencesWindow;

  QAction *saveAct;
  QAction *newAct;
  QAction *openAct;
  QMenu *fileMenu;
};

} // namespace mechanizm