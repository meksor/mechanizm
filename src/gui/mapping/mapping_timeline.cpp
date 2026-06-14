#include "gui/mapping/mapping_timeline.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <cmath>

#include <libopenshot/Fraction.h>

namespace mechanizm {

MappingTimeline::MappingTimeline(QWidget *parent)
    : AbstractTimelineWidget(parent) {
  setMinimumHeight(180);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

int MappingTimeline::wrapPointIndex(int point, int size,
                                    mechanizm::Mapping::WrapBehaviour wrapBehaviour) {
  if (size <= 0) {
    return 0;
  }

  if (wrapBehaviour == mechanizm::Mapping::WrapBehaviour::BOUNCE) {
    if (size <= 1) {
      return 0;
    }
    const int period = 2 * (size - 1);
    int mod = point % period;
    if (mod < 0) {
      mod += period;
    }
    return (mod < size) ? mod : period - mod;
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

void MappingTimeline::setCursorFrame(double frame) {
  hasCursorFrame = true;
  cursorFrame = frame;
  update();
}

void MappingTimeline::setBpm(double value) {
  bpm = value;
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
  openshot::Fraction fps = mapping->clip->source->reader->info.fps;
  const double framesPerMinute = 60.0 * fps.ToFloat();
  const double noteLengthInFrames = framesPerMinute / bpm;

  int point = 0;
  double currentFrame = rythmicPoints.front().frame;
  curve.push_back(QPointF(0.0, currentFrame));

  for (const auto &step : timeSeries) {
    const auto &timeStep = std::get<0>(step);
    const auto *channel = std::get<1>(step);

    if (channel == nullptr) {
      continue;
    }

    const double eventFrame = std::round(noteLengthInFrames * timeStep.note);
    curve.push_back(QPointF(eventFrame, currentFrame));

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

    point = wrapPointIndex(point, static_cast<int>(rythmicPoints.size()),
                           mapping->wrapBehaviour);
    currentFrame = rythmicPoints[point].frame;
    curve.push_back(QPointF(eventFrame, currentFrame));
  }
}

void MappingTimeline::paintTimeline(QPainter &painter, const QRect &plotArea) {
  if (curve.empty()) {
    drawCenteredMessage(painter, plotArea,
                        mapping == nullptr
                            ? tr("Select a mapping to view its timeline")
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

  drawHorizontalGrid(painter, plotArea, 5);

  QPainterPath path;
  path.moveTo(toScreen(curve.front()));
  for (std::size_t i = 1; i < curve.size(); ++i) {
    path.lineTo(toScreen(curve[i]));
  }

  painter.setPen(QPen(palette().primary, 2.0));
  painter.drawPath(path);

  painter.setBrush(palette().accent);
  painter.setPen(Qt::NoPen);
  for (const auto &point : curve) {
    const QPointF screenPoint = toScreen(point);
    painter.drawEllipse(screenPoint, 2.5, 2.5);
  }

  drawAxisLabels(painter, plotArea, tr("Frame position"), tr("Time"));

  if (hasCursorFrame) {
    const double clampedCursorXValue = qBound(minX, cursorFrame, maxX);
    const double xRatio = (clampedCursorXValue - minX) / (maxX - minX);
    const double cursorX = plotArea.left() + (xRatio * plotArea.width());

    double cursorYValue = curve.front().y();
    for (const auto &point : curve) {
      if (point.x() > clampedCursorXValue) {
        break;
      }
      cursorYValue = point.y();
    }

    const double clampedCursorYValue = qBound(minY, cursorYValue, maxY);
    const double yRatio = (clampedCursorYValue - minY) / (maxY - minY);
    const double cursorY = plotArea.bottom() - (yRatio * plotArea.height());

    painter.setPen(QPen(palette().cursor, 1.0, Qt::DashLine));
    painter.drawLine(plotArea.left(), static_cast<int>(cursorY),
                     plotArea.right(), static_cast<int>(cursorY));

    painter.setPen(QPen(palette().cursor, 1.5));
    painter.drawLine(static_cast<int>(cursorX), plotArea.top(),
                     static_cast<int>(cursorX), plotArea.bottom());

    painter.setBrush(palette().cursor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(cursorX, cursorY), 4.0, 4.0);
  }
}

} // namespace mechanizm