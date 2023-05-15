#include "gui/sequence/sequences_window.h"
#include "clip.h"
#include "json.h"
#include "project.h"
#include "source.h"
#include <QAction>
#include <QDebug>
#include <QMenuBar>

namespace mechanizm {

SequencesWindow::SequencesWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Sequences");
  createActions();
  createMenus();

  QWidget *widget = new QWidget;
  hbox = new QHBoxLayout(widget);

  sequenceTable = new mechanizm::SequenceTable();
  hbox->addWidget(sequenceTable);

  sequenceInfo = new mechanizm::SequenceInfo();
  sequenceInfo->setFixedWidth(200);
  hbox->addWidget(sequenceInfo);

  setCentralWidget(widget);
  this->setFixedSize(700, 300);

  connect(sequenceTable, &SequenceTable::selectSequence, sequenceInfo,
          &SequenceInfo::onSequenceSelected);
}

void SequencesWindow::createActions() {
  removeAct = new QAction(tr("&Remove Sequence"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected sequence"));

  connect(removeAct, &QAction::triggered, this,
          &SequencesWindow::removeSelectedSequence);
}

void SequencesWindow::createMenus() {
  sequenceMenu = this->menuBar()->addMenu(tr("&Sequence"));
  sequenceMenu->addAction(removeAct);
}

void SequencesWindow::removeSelectedSequence() {
  project->removeSequence(sequenceTable->getSelectedSequence());
}

} // namespace mechanizm