#include "mapping_table.h"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <string>
#include <vector>

namespace mechanizm {

MappingTable::MappingTable(QWidget *parent) : QTableWidget(parent) {
  this->setSelectionBehavior(SelectionBehavior::SelectRows);
  this->setColumnCount(2);
  this->setHorizontalHeaderLabels(QStringList({"Name", "Clip"}));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->horizontalHeader()->setStretchLastSection(true);
  connect(this, &MappingTable::currentCellChanged, this,
          &MappingTable::onCellChanged);
}

mechanizm::Mapping *MappingTable::getSelectedMapping() {
  return mappings[this->currentRow()];
};

void MappingTable::onCellChanged(int currentRow, int currentColumn,
                                  int previousRow, int previousColumn) {
  if (currentRow != previousRow && !mappings.empty()) {
    emit selectMapping(mappings[currentRow]);
  }
}

void MappingTable::onMappingsChanged(std::vector<mechanizm::Mapping *> m) {
  mappings = m;
  this->setRowCount(mappings.size());
  for (int i = 0; i < mappings.size(); i++) {
    mechanizm::Mapping *mapping = mappings[i];
    QTableWidgetItem *nameItem =
        new QTableWidgetItem(QString(mapping->name.c_str()));
    QTableWidgetItem *clipItem =
        new QTableWidgetItem(QString(mapping->clip->name.c_str()));

    std::vector<QTableWidgetItem *> items = {nameItem, clipItem};
    for (int j = 0; j < items.size(); j++) {
      items[j]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                         Qt::ItemIsEnabled);
      this->setItem(i, j, items[j]);
    }
  }
}

} // namespace mechanizm