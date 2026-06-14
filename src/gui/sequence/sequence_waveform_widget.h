#pragma once

#include "gui/mapping/abstract_timeline_widget.h"
#include "sequence.h"
#include <libopenshot/AudioWaveformer.h>

namespace mechanizm {

class SequenceWaveformWidget : public AbstractTimelineWidget {
  Q_OBJECT
public:
  explicit SequenceWaveformWidget(QWidget *parent = nullptr);

  void setSequence(mechanizm::Sequence *sequence);

protected:
  void paintTimeline(QPainter &painter, const QRect &plotArea) override;

private:
  void refreshWaveform();

  mechanizm::Sequence *sequence = nullptr;
  openshot::AudioWaveformData waveformData;
  bool hasWaveform = false;
};

} // namespace mechanizm