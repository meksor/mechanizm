#pragma once

#include "mapping.h"
#include <QWidget>
#include <vector>

namespace mechanizm {

class MappingTimeline : public QWidget {
  Q_OBJECT
public:
  explicit MappingTimeline(QWidget *parent = nullptr);

  void onMappingSelected(mechanizm::Mapping *mapping);
  void setCursorFrame(double frame);
  void setBpm(double value);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  void rebuildCurve();
  static int wrapPointIndex(int point, int size);

  mechanizm::Mapping *mapping = nullptr;
  std::vector<QPointF> curve;
  bool hasCursorFrame = false;
  double cursorFrame = 0.0;
  double bpm = 120.0;
};

} // namespace mechanizm