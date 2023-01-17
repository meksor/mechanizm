#include "gui/sources_window.h"
#include "clip.h"
#include "json.h"
#include "project.h"
#include "sequence.h"
#include "source.h"
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <qnamespace.h>

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
  importAct = new QAction(tr("&Import New Source"), this);
  importAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_I));
  importAct->setStatusTip(tr("Import a source file (video, midi)"));

  removeAct = new QAction(tr("&Remove Source"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected source file"));

  convertAct = new QAction(tr("&Convert Source"), this);
  convertAct->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return));
  convertAct->setStatusTip(
      tr("Convert the selected source file to it's mechanizm equivalent"));

  connect(importAct, &QAction::triggered, this, &SourcesWindow::importSource);
  connect(removeAct, &QAction::triggered, this,
          &SourcesWindow::removeSelectedSource);
  connect(convertAct, &QAction::triggered, this,
          &SourcesWindow::convertSelectedSource);
}

void SourcesWindow::createMenus() {
  sourceMenu = this->menuBar()->addMenu(tr("&Source"));
  sourceMenu->addAction(importAct);
  sourceMenu->addSeparator();
  sourceMenu->addAction(removeAct);
  sourceMenu->addAction(convertAct);
}

void SourcesWindow::importSource() {
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::AnyFile);
  // dialog.setNameFilters(SourcesWindow::nameFilters);
  dialog.setNameFilters(getNameFilters());

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

void SourcesWindow::convertSelectedSource() {
  mechanizm::Source *source = sourceTable->getSelectedSource();
  if (source->type == Source::Type::VIDEO) {
    mechanizm::id_t id = mechanizm::Clip::getNextId(project->clips);
    mechanizm::Clip *clip = new mechanizm::Clip(id, source);
    project->addClip(clip);
  } else if (source->type == Source::Type::MIDI) {
    mechanizm::id_t id = mechanizm::Sequence::getNextId(project->sequences);
    mechanizm::Sequence *sequence = new mechanizm::Sequence(id, source);
    project->addSequence(sequence);
  }
}

const QStringList getNameFilters() {
  QStringList res;
  auto dpNameMap = Source::getdpNameMap();
  for (auto const &[key, val] : Source::getExtentionMap()) {
    std::string dpName = dpNameMap.at(key);
    std::string extensions = "";
    for (auto const ext : val) {
      extensions += "*" + ext + " ";
    }
    extensions.resize(extensions.size() - 1);
    std::string nameFilter = dpName + " (" + extensions + ")";
    res.append(QString(nameFilter.c_str()));
  }
  return res;
}
const QStringList SourcesWindow::nameFilters = getNameFilters();
} // namespace mechanizm