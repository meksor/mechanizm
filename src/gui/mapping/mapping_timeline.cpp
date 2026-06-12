#include "gui/mapping/mapping_timeline.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPainterPath>

namespace mechanizm {

MappingTimeline::MappingTimeline(QWidget *parent) : QWidget(parent) {
  setMinimumHeight(180);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

int MappingTimeline::wrapPointIndex(int point, int size) {
  if (size <= 0) {
    return 0;
  }

  int wrapped = point % size;
  if (wrapped < 0) {
    wrapped += size;
  }
  return wrapped;
}

void MappingTimeline::onMappingSelected(mechanizm::Mapping *selectedMapping) {
  if (mapping != nullptr) {
    disconnect(mapping, &Mapping::updated, this, nullptr);
  }

  mapping = selectedMapping;

  if (mapping != nullptr) {
    connect(mapping, &Mapping::updated, this, [this]() {
      rebuildCurve();
      update();
    });
  }

  rebuildCurve();
  update();
}

void MappingTimeline::rebuildCurve() {
  curve.clear();

  if (mapping == nullptr || mapping->clip == nullptr ||
      mapping->clip->rythmicPoints.empty()) {
    return;
  }

  const auto &rythmicPoints = mapping->clip->rythmicPoints;
  auto timeSeries = mapping->getChannelTimeseries();

  int point = 0;
  double currentFrame = rythmicPoints.front().frame;
  curve.push_back(QPointF(0.0, currentFrame));

  for (const auto &step : timeSeries) {
    const auto &timeStep = std::get<0>(step);
    const auto *channel = std::get<1>(step);

    if (channel == nullptr) {
      continue;
    }

    const double eventNote = timeStep.note;
    curve.push_back(QPointF(eventNote, currentFrame));

    switch (channel->effect) {
    case mechanizm::Channel::Effect::INC:
      point += channel->value;
      break;
    case mechanizm::Channel::Effect::DEC:
      point -= channel->value;
      break;
    case mechanizm::Channel::Effect::SET:
      point = channel->value;
      break;
    }

    point = wrapPointIndex(point, static_cast<int>(rythmicPoints.size()));
    currentFrame = rythmicPoints[point].frame;
    curve.push_back(QPointF(eventNote, currentFrame));
  }
}

void MappingTimeline::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.fillRect(rect(), QColor(20, 24, 30));

  const int left = 48;
  const int right = 16;
  const int top = 16;
  const int bottom = 28;
  const QRect plotArea(left, top, width() - left - right,
                       height() - top - bottom);

  painter.setPen(QPen(QColor(54, 62, 74), 1));
  painter.drawRect(plotArea);

  if (curve.empty()) {
    painter.setPen(QColor(180, 185, 194));
    painter.drawText(plotArea, Qt::AlignCenter,
                     mapping == nullptr ? tr("Select a mapping to view its timeline")
                                        : tr("No channel curve available"));
    return;
  }

  double minX = curve.front().x();
  double maxX = curve.front().x();
  double minY = curve.front().y();
  double maxY = curve.front().y();

  for (const auto &point : curve) {
    minX = qMin(minX, point.x());
    maxX = qMax(maxX, point.x());
    minY = qMin(minY, point.y());
    maxY = qMax(maxY, point.y());
  }

  if (qFuzzyCompare(minX, maxX)) {
    maxX += 1.0;
  }
  if (qFuzzyCompare(minY, maxY)) {
    maxY += 1.0;
  }

  auto toScreen = [&](const QPointF &point) {
    const double xRatio = (point.x() - minX) / (maxX - minX);
    const double yRatio = (point.y() - minY) / (maxY - minY);
    const double x = plotArea.left() + (xRatio * plotArea.width());
    const double y = plotArea.bottom() - (yRatio * plotArea.height());
    return QPointF(x, y);
  };

  painter.setPen(QPen(QColor(70, 80, 96), 1, Qt::DashLine));
  for (int i = 1; i < 5; ++i) {
    const int y = plotArea.top() + (plotArea.height() * i / 5);
    painter.drawLine(plotArea.left(), y, plotArea.right(), y);
  }

  QPainterPath path;
  path.moveTo(toScreen(curve.front()));
  for (std::size_t i = 1; i < curve.size(); ++i) {
    path.lineTo(toScreen(curve[i]));
  }

  painter.setPen(QPen(QColor(102, 193, 255), 2.0));
  painter.drawPath(path);

  painter.setBrush(QColor(255, 199, 95));
  painter.setPen(Qt::NoPen);
  for (const auto &point : curve) {
    const QPointF screenPoint = toScreen(point);
    painter.drawEllipse(screenPoint, 2.5, 2.5);
  }

  painter.setPen(QColor(180, 185, 194));
  painter.drawText(6, plotArea.top() + 12, tr("Frame position"));
  painter.drawText(plotArea.right() - 70, height() - 8, tr("Time"));
}

} // namespace mechanizm