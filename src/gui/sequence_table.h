#pragma once

#include "json.h"
#include "project.h"
#include "sequence.h"
#include <QTableWidget>

namespace mechanizm {

class SequenceTable : public QTableWidget {
  Q_OBJECT
public:
  SequenceTable(QWidget *parent = nullptr);

  void onSequencesChanged(std::vector<mechanizm::Sequence *>);
  void onCellChanged(int currentRow, int currentColumn, int previousRow,
                     int previousColumn);
  mechanizm::Sequence *getSelectedSequence();

signals:
  void selectSequence(mechanizm::Sequence *);

private:
  std::vector<mechanizm::Sequence *> sequences;
};

} // namespace mechanizm
