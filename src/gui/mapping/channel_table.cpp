#include "gui/mapping/channel_table.h"
#include "mapping.h"
#include <QHeaderView>
#include <QString>
#include <QTableWidget>
#include <string>
#include <vector>

namespace mechanizm {

ChannelTable::ChannelTable(QWidget *parent) : QTableWidget(parent) {
  this->setSelectionBehavior(SelectionBehavior::SelectRows);
  this->setColumnCount(2);
  this->setHorizontalHeaderLabels(QStringList({"Name", "Sequence"}));
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->horizontalHeader()->setStretchLastSection(true);
  connect(this, &ChannelTable::currentCellChanged, this,
          &ChannelTable::onCellChanged);
}

mechanizm::Channel ChannelTable::getSelectedChannel() {
  return channels[this->currentRow()];
};

void ChannelTable::onCellChanged(int currentRow, int currentColumn,
                                 int previousRow, int previousColumn) {
  if (currentRow != previousRow) {
    emit selectChannel(&channels[this->currentRow()]);
  }
}

void ChannelTable::onMappingUpdated(mechanizm::Mapping *mapping) {
  channels = mapping->channels;
  this->setRowCount(channels.size());
  for (int i = 0; i < channels.size(); i++) {
    mechanizm::Channel channel = channels[i];
    QTableWidgetItem *nameItem =
        new QTableWidgetItem(QString(channel.name.c_str()));
    QTableWidgetItem *seqItem =
        new QTableWidgetItem(channel.sequence->name.c_str());

    std::vector<QTableWidgetItem *> items = {nameItem, seqItem};
    for (int j = 0; j < items.size(); j++) {
      items[j]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                         Qt::ItemIsEnabled);
      this->setItem(i, j, items[j]);
    }
  }
}

} // namespace mechanizm