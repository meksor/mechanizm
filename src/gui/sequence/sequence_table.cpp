#include "sequence_table.h"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <string>
#include <vector>

namespace mechanizm {

SequenceTable::SequenceTable(QWidget *parent) : QTableWidget(parent) {
  this->setSelectionBehavior(SelectionBehavior::SelectRows);
  this->setColumnCount(2);
  this->setHorizontalHeaderLabels(QStringList({"Name", "Audio"}));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->horizontalHeader()->setStretchLastSection(true);
  connect(this, &SequenceTable::currentCellChanged, this,
          &SequenceTable::onCellChanged);
}

QString SequenceTable::displayAudioName(
    const mechanizm::Sequence *sequence) const {
  if (sequence == nullptr || sequence->audioSource == nullptr) {
    return tr("None");
  }
  return QString::fromStdString(sequence->audioSource->name);
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
    QTableWidgetItem *audioItem =
        new QTableWidgetItem(displayAudioName(sequence));

    std::vector<QTableWidgetItem *> items = {nameItem, audioItem};
    for (int j = 0; j < items.size(); j++) {
      items[j]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                         Qt::ItemIsEnabled);
      this->setItem(i, j, items[j]);
    }
  }
}

} // namespace mechanizm