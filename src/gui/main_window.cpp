#include "gui/main_window.h"
#include "gui/project_widget.h"
#include "project.h"
#include <QFileDialog>

namespace mechanizm {

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Project");
  createActions();
  createMenus();
  projectWidget = new mechanizm::ProjectWidget(this);
  projectWidget->hide();
  setCentralWidget(projectWidget);
  connect(this, &MainWindow::projectChanged, projectWidget,
          &mechanizm::ProjectWidget::onProjectChanged);

  sourcesWindow = new mechanizm::SourcesWindow(this);
  sourcesWindow->hide();
  connect(this, &MainWindow::projectChanged, sourcesWindow,
          &mechanizm::SourcesWindow::onProjectChanged);

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
    p->setDefaults();
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