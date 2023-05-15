#pragma once

#include "clip.h"
#include "project.h"
#include <QTableWidget>

namespace mechanizm {

class ClipTable : public QTableWidget {
  Q_OBJECT
public:
  ClipTable(QWidget *parent = nullptr);

  void onClipsChanged(std::vector<mechanizm::Clip *>);
  void onCellChanged(int currentRow, int currentColumn, int previousRow,
                     int previousColumn);
  mechanizm::Clip *getSelectedClip();

signals:
  void selectClip(mechanizm::Clip *);

private:
  std::vector<mechanizm::Clip *> clips;
};

} // namespace mechanizm