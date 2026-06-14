#include "gui/mapping/abstract_timeline_widget.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QRect>

namespace mechanizm {

AbstractTimelineWidget::AbstractTimelineWidget(QWidget *parent)
    : QWidget(parent) {}

QRect AbstractTimelineWidget::buildPlotArea(const PlotMargins &margins) const {
  return QRect(margins.left, margins.top,
               width() - margins.left - margins.right,
               height() - margins.top - margins.bottom);
}

void AbstractTimelineWidget::drawPlotFrame(QPainter &painter,
                                           const QRect &plotArea) const {
  painter.setPen(QPen(colors.border, 1));
  painter.drawRect(plotArea);
}

void AbstractTimelineWidget::drawHorizontalGrid(QPainter &painter,
                                                const QRect &plotArea,
                                                int segments) const {
  if (segments <= 1) {
    return;
  }

  painter.setPen(QPen(colors.grid, 1, Qt::DashLine));
  for (int i = 1; i < segments; ++i) {
    const int y = plotArea.top() + (plotArea.height() * i / segments);
    painter.drawLine(plotArea.left(), y, plotArea.right(), y);
  }
}

void AbstractTimelineWidget::drawAxisLabels(QPainter &painter,
                                            const QRect &plotArea,
                                            const QString &leftLabel,
                                            const QString &bottomLabel) const {
  painter.setPen(colors.text);
  painter.drawText(6, plotArea.top() + 12, leftLabel);
  painter.drawText(plotArea.right() - 70, height() - 8, bottomLabel);
}

void AbstractTimelineWidget::drawCenteredMessage(QPainter &painter,
                                                 const QRect &plotArea,
                                                 const QString &message) const {
  painter.setPen(colors.text);
  painter.drawText(plotArea, Qt::AlignCenter, message);
}

void AbstractTimelineWidget::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.fillRect(rect(), colors.background);

  const QRect plotArea = buildPlotArea(PlotMargins{});
  if (plotArea.width() <= 0 || plotArea.height() <= 0) {
    return;
  }

  drawPlotFrame(painter, plotArea);
  paintTimeline(painter, plotArea);
}

} // namespace mechanizm
