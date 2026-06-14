#pragma once

#include "clip.h"
#include "gui/mapping/abstract_timeline_widget.h"

class QPainter;
class QRect;

namespace mechanizm {

class ClipTimeline : public AbstractTimelineWidget {
  Q_OBJECT
public:
  explicit ClipTimeline(QWidget *parent = nullptr);

  void onClipSelected(mechanizm::Clip *selectedClip);
  void setCursorFrame(double frame);

protected:
  void paintTimeline(QPainter &painter, const QRect &plotArea) override;

private:
  double toScreenX(double frame, const QRect &plotArea, int sourceStart,
                   int sourceEnd, int clipStartFrame, int clipEndFrame) const;

  mechanizm::Clip *clip = nullptr;
  bool hasCursorFrame = false;
  double cursorFrame = 0.0;
};

} // namespace mechanizm
