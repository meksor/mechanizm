#include "gui/sequence/sequence_timeline_widget.h"

#include <QPainter>
#include <QPen>

#include <algorithm>

namespace mechanizm {

SequenceTimelineWidget::SequenceTimelineWidget(QWidget *parent)
    : AbstractTimelineWidget(parent) {}

void SequenceTimelineWidget::setSequence(mechanizm::Sequence *newSequence) {
  sequence = newSequence;
  update();
}

void SequenceTimelineWidget::paintTimeline(QPainter &painter,
                                          const QRect &plotArea) {
  drawHorizontalGrid(painter, plotArea, 4);
  drawAxisLabels(painter, plotArea, tr("Hits"), tr("Beats"));

  if (sequence == nullptr || sequence->timeSteps.empty()) {
    drawCenteredMessage(painter, plotArea, tr("No sequence events"));
    return;
  }

  const double maxNote =
      std::max(1.0, sequence->timeSteps.back().note);
  const int baseline = plotArea.bottom() - plotArea.height() / 5;

  painter.setPen(QPen(palette().highlight, 2));
  painter.drawLine(plotArea.left(), baseline, plotArea.right(), baseline);

  painter.setPen(QPen(palette().accent, 1.5));
  painter.setBrush(palette().primary);
  for (const auto &timeStep : sequence->timeSteps) {
    const double ratio = std::clamp(timeStep.note / maxNote, 0.0, 1.0);
    const int x = plotArea.left() +
                  static_cast<int>(ratio * static_cast<double>(plotArea.width()));
    const int top = plotArea.top() + plotArea.height() / 5;
    painter.drawLine(x, baseline, x, top);
    painter.drawEllipse(QPointF(x, top), 3.0, 3.0);
  }
}

} // namespace mechanizm