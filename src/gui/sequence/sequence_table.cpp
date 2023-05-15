#include "sequence_table.h"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <string>
#include <vector>

namespace mechanizm {

SequenceTable::SequenceTable(QWidget *parent) : QTableWidget(parent) {
  this->setSelectionBehavior(SelectionBehavior::SelectRows);
  this->setColumnCount(1);
  this->setHorizontalHeaderLabels(QStringList({"Name"}));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->horizontalHeader()->setStretchLastSection(true);
  connect(this, &SequenceTable::currentCellChanged, this,
          &SequenceTable::onCellChanged);
}

mechanizm::Sequence *SequenceTable::getSelectedSequence() {
  return sequences[this->currentRow()];
};

void SequenceTable::onCellChanged(int currentRow, int currentColumn,
                                  int previousRow, int previousColumn) {
  if (currentRow != previousRow && !sequences.empty()) {
    emit selectSequence(sequences[currentRow]);
  }
}

void SequenceTable::onSequencesChanged(std::vector<mechanizm::Sequence *> s) {
  sequences = s;
  this->setRowCount(sequences.size());
  for (int i = 0; i < sequences.size(); i++) {
    mechanizm::Sequence *sequence = sequences[i];
    QTableWidgetItem *nameItem =
        new QTableWidgetItem(QString(sequence->name.c_str()));

    std::vector<QTableWidgetItem *> items = {nameItem};
    for (int j = 0; j < items.size(); j++) {
      items[j]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                         Qt::ItemIsEnabled);
      this->setItem(i, j, items[j]);
    }
  }
}

} // namespace mechanizm