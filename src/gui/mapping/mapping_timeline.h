#pragma once

#include "mapping.h"
#include "gui/mapping/abstract_timeline_widget.h"
#include <vector>

class QPainter;
class QRect;

namespace mechanizm {

class MappingTimeline : public AbstractTimelineWidget {
  Q_OBJECT
public:
  explicit MappingTimeline(QWidget *parent = nullptr);

  void onMappingSelected(mechanizm::Mapping *mapping);
  void setCursorFrame(double frame);
  void setBpm(double value);

protected:
  void paintTimeline(QPainter &painter, const QRect &plotArea) override;

private:
  void rebuildCurve();
  static int wrapPointIndex(int point, int size,
                            mechanizm::Mapping::WrapBehaviour wrapBehaviour);

  mechanizm::Mapping *mapping = nullptr;
  std::vector<QPointF> curve;
  bool hasCursorFrame = false;
  double cursorFrame = 0.0;
  double bpm = 120.0;
};

} // namespace mechanizm