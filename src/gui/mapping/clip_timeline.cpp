#include "gui/mapping/clip_timeline.h"

#include <QPainter>
#include <QPen>
#include <algorithm>
#include <cmath>

namespace mechanizm {

ClipTimeline::ClipTimeline(QWidget *parent) : AbstractTimelineWidget(parent) {
  setMinimumHeight(140);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
}

void ClipTimeline::onClipSelected(mechanizm::Clip *selectedClip) {
  if (clip != nullptr) {
    disconnect(clip, &Clip::updated, this, nullptr);
  }

  clip = selectedClip;

  if (clip != nullptr) {
    connect(clip, &Clip::updated, this, [this]() { update(); });
  }

  update();
}

void ClipTimeline::setCursorFrame(double frame) {
  hasCursorFrame = true;
  cursorFrame = frame;
  update();
}

double ClipTimeline::toScreenX(double frame, const QRect &plotArea, int sourceStart,
                               int sourceEnd, int clipStartFrame,
                               int clipEndFrame) const {
  if (sourceEnd <= sourceStart) {
    return plotArea.left();
  }

  const double sourceLength = static_cast<double>(sourceEnd - sourceStart);
  const double innerLength =
      std::max(1.0, static_cast<double>(clipEndFrame - clipStartFrame));

  const double leftOuterLength =
      std::max(0.0, static_cast<double>(clipStartFrame - sourceStart));
  const double rightOuterLength =
      std::max(0.0, static_cast<double>(sourceEnd - clipEndFrame));

  const double outerLength = leftOuterLength + rightOuterLength;
  const double outerWeight = (outerLength > 0.0)
                                 ? std::min(0.45, std::max(0.15, outerLength / sourceLength))
                                 : 0.0;

  const double leftWeight = (outerLength > 0.0)
                                ? outerWeight * (leftOuterLength / outerLength)
                                : 0.0;
  const double rightWeight = (outerLength > 0.0)
                                 ? outerWeight * (rightOuterLength / outerLength)
                                 : 0.0;
  const double centerWeight = std::max(0.1, 1.0 - leftWeight - rightWeight);

  const double clampedFrame =
      std::clamp(frame, static_cast<double>(sourceStart), static_cast<double>(sourceEnd));

  double ratio = 0.0;
  if (clampedFrame <= clipStartFrame && leftOuterLength > 0.0) {
    const double local = (clampedFrame - sourceStart);
    const double den = std::log1p(leftOuterLength);
    const double scaled = (den > 0.0) ? (std::log1p(local) / den) : 0.0;
    ratio = leftWeight * scaled;
  } else if (clampedFrame >= clipEndFrame && rightOuterLength > 0.0) {
    const double local = (clampedFrame - clipEndFrame);
    const double den = std::log1p(rightOuterLength);
    const double scaled = (den > 0.0) ? (std::log1p(local) / den) : 0.0;
    ratio = leftWeight + centerWeight + (rightWeight * scaled);
  } else {
    const double local = (clampedFrame - clipStartFrame);
    ratio = leftWeight + centerWeight * (local / innerLength);
  }

  return plotArea.left() + (ratio * plotArea.width());
}

void ClipTimeline::paintTimeline(QPainter &painter, const QRect &plotArea) {
  if (clip == nullptr || clip->source == nullptr || clip->source->reader == nullptr) {
    drawCenteredMessage(painter, plotArea, tr("Select a clip to view its timeline"));
    return;
  }

  const int sourceStart = 0;
  const int sourceEnd = std::max(1l, clip->source->reader->info.video_length);
  int clipStartFrame = sourceStart;
  int clipEndFrame = sourceEnd;
  if (!clip->rythmicPoints.empty()) {
    clipStartFrame = std::clamp(static_cast<int>(clip->getFirstFrame()),
                                sourceStart, sourceEnd - 1);
    clipEndFrame =
        std::clamp(static_cast<int>(clip->getLastFrame()), clipStartFrame, sourceEnd);
  }

  const double startX = toScreenX(clipStartFrame, plotArea, sourceStart, sourceEnd,
                                  clipStartFrame, clipEndFrame);
  const double endX = toScreenX(clipEndFrame, plotArea, sourceStart, sourceEnd,
                                clipStartFrame, clipEndFrame);

  painter.setPen(Qt::NoPen);
  painter.setBrush(palette().outsideRegion);
  painter.drawRect(QRectF(plotArea.left(), plotArea.top(),
                          std::max(0.0, startX - plotArea.left()), plotArea.height()));
  painter.drawRect(QRectF(endX, plotArea.top(),
                          std::max(0.0, plotArea.right() - endX), plotArea.height()));

  drawHorizontalGrid(painter, plotArea, 4);

  const int midY = plotArea.center().y();
  painter.setPen(QPen(palette().primary, 2.0));
  painter.drawLine(plotArea.left(), midY, plotArea.right(), midY);

  painter.setPen(QPen(palette().highlight, 2.0));
  painter.drawLine(QPointF(startX, plotArea.top()), QPointF(startX, plotArea.bottom()));
  painter.drawLine(QPointF(endX, plotArea.top()), QPointF(endX, plotArea.bottom()));

  painter.setPen(palette().text);
  painter.drawText(QPointF(startX + 4.0, plotArea.top() + 14.0),
                   tr("Start %1").arg(clipStartFrame));
  painter.drawText(QPointF(endX + 4.0, plotArea.top() + 14.0),
                   tr("End %1").arg(clipEndFrame));

  painter.setBrush(palette().accent);
  painter.setPen(Qt::NoPen);
  for (const auto &point : clip->rythmicPoints) {
    const double pointX = toScreenX(point.frame, plotArea, sourceStart, sourceEnd,
                                    clipStartFrame, clipEndFrame);
    painter.drawEllipse(QPointF(pointX, midY), 3.2, 3.2);
  }

  if (hasCursorFrame) {
    const double cursorX = toScreenX(cursorFrame, plotArea, sourceStart, sourceEnd,
                                     clipStartFrame, clipEndFrame);
    painter.setPen(QPen(palette().cursor, 1.5, Qt::DashLine));
    painter.drawLine(QPointF(cursorX, plotArea.top()), QPointF(cursorX, plotArea.bottom()));

    painter.setBrush(palette().cursor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(cursorX, midY), 4.0, 4.0);
  }

  drawAxisLabels(painter, plotArea, tr("Source frame"), tr("Timeline"));
}

} // namespace mechanizm
