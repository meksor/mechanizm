#pragma once

#include "json.h"
#include "mapping.h"
#include "project.h"
#include <QTableWidget>

namespace mechanizm {

class ChannelTable : public QTableWidget {
  Q_OBJECT
public:
  ChannelTable(QWidget *parent = nullptr);

  void onMappingUpdated(mechanizm::Mapping *m);
  void onCellChanged(int currentRow, int currentColumn, int previousRow,
                     int previousColumn);
  mechanizm::Channel getSelectedChannel();

signals:
  void selectChannel(mechanizm::Channel *channel);

private:
  std::vector<mechanizm::Channel> channels;
};

} // namespace mechanizm
