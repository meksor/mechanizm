#include "gui/main_window.h"
#include "gui/mapping/mapping_table.h"
#include <QFileDialog>

namespace mechanizm {

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Project");

  createActions();
  createMenus();

  projectWidget = new mechanizm::ProjectWidget(this);
  projectWidget->hide();
  projectWidget->addAction(saveAct);
  setCentralWidget(projectWidget);
  connect(this, &MainWindow::projectChanged, projectWidget,
          &mechanizm::ProjectWidget::onProjectChanged);

  sourcesWindow = new mechanizm::SourcesWindow(this);
  sourcesWindow->hide();
  sourcesWindow->addAction(saveAct);
  connect(this, &MainWindow::projectChanged, sourcesWindow,
          &mechanizm::SourcesWindow::onProjectChanged);

  clipsWindow = new mechanizm::ClipsWindow(this);
  clipsWindow->hide();
  clipsWindow->addAction(saveAct);
  connect(this, &MainWindow::projectChanged, clipsWindow,
          &mechanizm::ClipsWindow::onProjectChanged);

  clipEditorWindow = new mechanizm::ClipEditorWindow(this);
  clipEditorWindow->hide();
  clipEditorWindow->addAction(saveAct);
  connect(clipsWindow->clipTable, &ClipTable::selectClip, clipEditorWindow,
          &ClipEditorWindow::onClipSelected);

  sequencesWindow = new mechanizm::SequencesWindow(this);
  sequencesWindow->hide();
  sequencesWindow->addAction(saveAct);
  connect(this, &MainWindow::projectChanged, sequencesWindow,
          &mechanizm::SequencesWindow::onProjectChanged);

  mappingsWindow = new mechanizm::MappingsWindow(this);
  mappingsWindow->hide();
  mappingsWindow->addAction(saveAct);
  connect(this, &MainWindow::projectChanged, mappingsWindow,
          &mechanizm::MappingsWindow::onProjectChanged);

  mappingEditorWindow = new mechanizm::MappingEditorWindow(this);
  connect(sequencesWindow->sequenceTable,
          &mechanizm::SequenceTable::selectSequence, mappingEditorWindow,
          &mechanizm::MappingEditorWindow::onSequenceSelected);
  connect(mappingsWindow->mappingTable, &mechanizm::MappingTable::selectMapping,
          mappingEditorWindow,
          &mechanizm::MappingEditorWindow::onMappingSelected);
  connect(this, &MainWindow::projectChanged, mappingEditorWindow,
          &mechanizm::MappingEditorWindow::onProjectChanged);

  this->setFixedSize(300, 180);
}

void MainWindow::createActions() {
  newAct = new QAction(tr("&New Project"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new project"));

  openAct = new QAction(tr("&Open Project"), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing project"));

  saveAct = new QAction(tr("&Save Project"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save current project"));

  connect(newAct, &QAction::triggered, this, &MainWindow::newProject);
  connect(openAct, &QAction::triggered, this, &MainWindow::openProject);
  connect(saveAct, &QAction::triggered, this, &MainWindow::saveProject);
}

void MainWindow::createMenus() {
  fileMenu = this->menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addSeparator();
  fileMenu->addAction(saveAct);
}

void MainWindow::changeProject(mechanizm::Project *p) {
  project = p;
  projectWidget->show();
  sourcesWindow->show();
  clipsWindow->show();
  clipEditorWindow->show();
  sequencesWindow->show();
  mappingsWindow->show();
  mappingEditorWindow->show();

  emit projectChanged(project);
  project->emitAll();
}

void MainWindow::saveProject() { project->saveToDisk(); }
void MainWindow::newProject() {
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::Directory);
  int result = dialog.exec();
  QString dirPath;
  if (result) {
    dirPath = dialog.selectedFiles()[0];
    QString name = dirPath.mid(dirPath.lastIndexOf("/") + 1);
    QDir dir = QDir(dirPath);

    if (project != nullptr) {
      delete project;
    }

    mechanizm::Project *p = new mechanizm::Project(dir, name.toStdString());
    changeProject(p);
  }
}

void MainWindow::openProject() {
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::Directory);
  dialog.setNameFilter(tr("Mechanizm Project Directory"));
  int result = dialog.exec();
  QString dirPath;
  if (result) {
    dirPath = dialog.selectedFiles()[0];
    QDir dir = QDir(dirPath);

    if (project != nullptr) {
      delete project;
    }

    changeProject(new mechanizm::Project(dir));
  }
}
} // namespace mechanizm