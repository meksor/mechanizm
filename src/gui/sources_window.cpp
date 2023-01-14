#include "gui/sources_window.h"
#include "json.h"
#include "project.h"
#include "source.h"
#include <QAction>
#include <QFileDialog>

namespace mechanizm {

SourcesWindow::SourcesWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Sources");
  createActions();
  createMenus();
  QWidget *widget = new QWidget;

  hbox = new QHBoxLayout(widget);
  sourceTable = new mechanizm::SourceTable();
  hbox->addWidget(sourceTable);

  sourceInfo = new mechanizm::SourceInfo();
  sourceInfo->setFixedWidth(200);
  hbox->addWidget(sourceInfo);

  setCentralWidget(widget);
  this->setFixedSize(700, 300);

  connect(sourceTable, &SourceTable::selectSource, sourceInfo,
          &SourceInfo::onSourceSelected);
}

void SourcesWindow::createActions() {
  importAct = new QAction(tr("&Import source"), this);
  importAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_I));
  importAct->setStatusTip(tr("Import a source file (video, midi)"));

  removeAct = new QAction(tr("&Remove source"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected source file"));

  connect(importAct, &QAction::triggered, this, &SourcesWindow::importSource);
  connect(removeAct, &QAction::triggered, this,
          &SourcesWindow::removeSelectedSource);
}

void SourcesWindow::createMenus() {
  sourceMenu = this->menuBar()->addMenu(tr("&Source"));
  sourceMenu->addAction(importAct);
  sourceMenu->addSeparator();
  sourceMenu->addAction(removeAct);
}

void SourcesWindow::importSource() {
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilters(SourcesWindow::nameFilters);
  int result = dialog.exec();
  QString dirPath;
  if (result) {
    QString filePath = dialog.selectedFiles()[0];
    mechanizm::id_t id = Source::getNextId(project->sources);

    mechanizm::Source *source =
        new mechanizm::Source(id, filePath.toStdString());
    project->addSource(source);
  }
}

void SourcesWindow::removeSelectedSource() {
  project->removeSource(sourceTable->getSelectedSource());
}

const QStringList getNameFilters(Source::ext_map_t extensionMap) {
  QStringList res;
  for (auto const &[key, val] : extensionMap) {
    std::string dpName = Source::dpNameMap.at(key);
    std::string extensions = "";
    for (auto const ext : val) {
      extensions += " *" + ext + ",";
    }
    std::string nameFilter = dpName + " (" + extensions + ")";
    res.append(QString(nameFilter.c_str()));
  }
  return res;
}
const QStringList SourcesWindow::nameFilters =
    getNameFilters(Source::extensionMap);
} // namespace mechanizm