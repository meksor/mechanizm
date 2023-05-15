#pragma once

#include "clip.h"
#include "json.h"
#include "project.h"
#include <QTableWidget>

namespace mechanizm {

class RythmicPointTable : public QTableWidget {
  Q_OBJECT
public:
  RythmicPointTable(QWidget *parent = nullptr);

  void onClipUpdated(mechanizm::Clip *clip);
  void onCellChanged(int currentRow, int currentColumn, int previousRow,
                     int previousColumn);
  mechanizm::RythmicPoint getSelectedRythmicPoint();

signals:
  void selectRythmicPoint(mechanizm::RythmicPoint rythmicPoint);

private:
  std::vector<mechanizm::RythmicPoint> rythmicPoints;
};

} // namespace mechanizm
