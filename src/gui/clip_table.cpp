#include "clip_table.h"
#include "clip.h"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <string>
#include <vector>

namespace mechanizm {

ClipTable::ClipTable(QWidget *parent) : QTableWidget(parent) {
  this->setSelectionBehavior(SelectionBehavior::SelectRows);
  this->setColumnCount(2);
  this->setHorizontalHeaderLabels(QStringList({"Name", "Source"}));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->horizontalHeader()->setStretchLastSection(true);
  connect(this, &ClipTable::currentCellChanged, this,
          &ClipTable::onCellChanged);
}

mechanizm::Clip *ClipTable::getSelectedClip() {
  return clips[this->currentRow()];
};

void ClipTable::onCellChanged(int currentRow, int currentColumn,
                              int previousRow, int previousColumn) {
  if (currentRow != previousRow) {
    emit selectClip(clips[currentRow]);
  }
}

void ClipTable::onClipsChanged(std::vector<mechanizm::Clip *> c) {
  clips = c;
  this->setRowCount(clips.size());
  for (int i = 0; i < clips.size(); i++) {
    mechanizm::Clip *clip = clips[i];
    QTableWidgetItem *nameItem =
        new QTableWidgetItem(QString(clip->name.c_str()));
    QTableWidgetItem *sourceItem =
        new QTableWidgetItem(QString(clip->source->name.c_str()));

    std::vector<QTableWidgetItem *> items = {nameItem, sourceItem};
    for (int j = 0; j < items.size(); j++) {
      items[j]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                         Qt::ItemIsEnabled);
      this->setItem(i, j, items[j]);
    }
  }
}

} // namespace mechanizm