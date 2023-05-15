#include "source_table.h"
#include "source.h"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <string>
#include <vector>

namespace mechanizm {

SourceTable::SourceTable(QWidget *parent) : QTableWidget(parent) {
  this->setSelectionBehavior(SelectionBehavior::SelectRows);
  this->setColumnCount(3);
  this->setHorizontalHeaderLabels(QStringList({"Name", "Type", "Path"}));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->horizontalHeader()->setStretchLastSection(true);
  connect(this, &SourceTable::currentCellChanged, this,
          &SourceTable::onCellChanged);
}

mechanizm::Source *SourceTable::getSelectedSource() {
  return sources[this->currentRow()];
};

void SourceTable::onCellChanged(int currentRow, int currentColumn,
                                int previousRow, int previousColumn) {
  if (currentRow != previousRow && !sources.empty()) {
    emit selectSource(sources[currentRow]);
  }
}

void SourceTable::onSourcesChanged(std::vector<mechanizm::Source *> s) {
  sources = s;
  auto dpNameMap = Source::getdpNameMap();
  this->setRowCount(sources.size());
  for (int i = 0; i < sources.size(); i++) {
    mechanizm::Source *source = sources[i];
    QTableWidgetItem *nameItem =
        new QTableWidgetItem(QString(source->name.c_str()));
    std::string typeDp = dpNameMap.at(source->type);
    QTableWidgetItem *typeItem = new QTableWidgetItem(QString(typeDp.c_str()));
    QTableWidgetItem *pathItem =
        new QTableWidgetItem(QString(source->path.c_str()));

    std::vector<QTableWidgetItem *> items = {nameItem, typeItem, pathItem};
    for (int j = 0; j < items.size(); j++) {
      items[j]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                         Qt::ItemIsEnabled);
      this->setItem(i, j, items[j]);
    }
  }
}

} // namespace mechanizm