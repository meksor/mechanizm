#pragma once

#include "json.h"
#include "project.h"
#include "source.h"
#include <QTableWidget>

namespace mechanizm {

class SourceTable : public QTableWidget {
  Q_OBJECT
public:
  SourceTable(QWidget *parent = nullptr);

  void onSourcesChanged(std::vector<mechanizm::Source *> sources);
  void onCellChanged(int currentRow, int currentColumn, int previousRow,
                     int previousColumn);
  mechanizm::Source *getSelectedSource();

signals:
  void selectSource(mechanizm::Source *source);

private:
  std::vector<mechanizm::Source *> sources;
};

} // namespace mechanizm
