#pragma once

#include "gui/mapping/abstract_timeline_widget.h"
#include "sequence.h"

namespace mechanizm {

class SequenceTimelineWidget : public AbstractTimelineWidget {
  Q_OBJECT
public:
  explicit SequenceTimelineWidget(QWidget *parent = nullptr);

  void setSequence(mechanizm::Sequence *sequence);

protected:
  void paintTimeline(QPainter &painter, const QRect &plotArea) override;

private:
  mechanizm::Sequence *sequence = nullptr;
};

} // namespace mechanizm