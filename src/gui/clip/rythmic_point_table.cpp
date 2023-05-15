#include "gui/clip/rythmic_point_table.h"
#include "clip.h"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <string>
#include <vector>

namespace mechanizm {

RythmicPointTable::RythmicPointTable(QWidget *parent) : QTableWidget(parent) {
  this->setSelectionBehavior(SelectionBehavior::SelectRows);
  this->setColumnCount(2);
  this->setHorizontalHeaderLabels(QStringList({"Frame #", "Timestamp"}));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->horizontalHeader()->setStretchLastSection(true);
  connect(this, &RythmicPointTable::currentCellChanged, this,
          &RythmicPointTable::onCellChanged);
}

mechanizm::RythmicPoint RythmicPointTable::getSelectedRythmicPoint() {
  return rythmicPoints[this->currentRow()];
};

void RythmicPointTable::onCellChanged(int currentRow, int currentColumn,
                                      int previousRow, int previousColumn) {
  if (currentRow != previousRow) {
    emit selectRythmicPoint(rythmicPoints[this->currentRow()]);
  }
}

void RythmicPointTable::onClipUpdated(mechanizm::Clip *clip) {
  rythmicPoints = clip->rythmicPoints;
  this->setRowCount(rythmicPoints.size());
  for (int i = 0; i < rythmicPoints.size(); i++) {
    mechanizm::RythmicPoint rythmicPoint = rythmicPoints[i];
    QTableWidgetItem *fnItem =
        new QTableWidgetItem(tr("%1").arg(rythmicPoint.frame));
    QTableWidgetItem *tsItem = new QTableWidgetItem(
        tr("%1").arg(rythmicPoint.getSeconds(clip->source->reader->info)));

    std::vector<QTableWidgetItem *> items = {fnItem, tsItem};
    for (int j = 0; j < items.size(); j++) {
      items[j]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                         Qt::ItemIsEnabled);
      this->setItem(i, j, items[j]);
    }
  }
}

} // namespace mechanizm