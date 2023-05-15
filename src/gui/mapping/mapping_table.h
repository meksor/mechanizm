#pragma once

#include "json.h"
#include "project.h"
#include "mapping.h"
#include <QTableWidget>

namespace mechanizm {

class MappingTable : public QTableWidget {
  Q_OBJECT
public:
  MappingTable(QWidget *parent = nullptr);

  void onMappingsChanged(std::vector<mechanizm::Mapping *>);
  void onCellChanged(int currentRow, int currentColumn, int previousRow,
                     int previousColumn);
  mechanizm::Mapping *getSelectedMapping();

signals:
  void selectMapping(mechanizm::Mapping *);

private:
  std::vector<mechanizm::Mapping *> mappings;
};

} // namespace mechanizm
