#pragma once

#include "gui/sequence_info.h"
#include "gui/sequence_table.h"
#include "project.h"
#include "sequence.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <vector>

namespace mechanizm {

class SequencesWindow : public QMainWindow {
  Q_OBJECT
public:
  SequencesWindow(QWidget *parent = nullptr,
                  Qt::WindowFlags flags = Qt::WindowFlags());

  void createMenus();
  void createActions();

  void removeSelectedSequence();

  void onProjectChanged(mechanizm::Project *p) {
    project = p;
    connect(project, &Project::sequencesChanged, sequenceTable,
            &mechanizm::SequenceTable::onSequencesChanged);
  };

private:
  mechanizm::SequenceTable *sequenceTable;
  mechanizm::SequenceInfo *sequenceInfo;
  mechanizm::Project *project;

  QHBoxLayout *hbox;
  QAction *removeAct;
  QMenu *sequenceMenu;
};
} // namespace mechanizm