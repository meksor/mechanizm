#include "gui/sequence/sequence_waveform_widget.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include <algorithm>

namespace mechanizm {

SequenceWaveformWidget::SequenceWaveformWidget(QWidget *parent)
    : AbstractTimelineWidget(parent) {}

void SequenceWaveformWidget::setSequence(mechanizm::Sequence *newSequence) {
  sequence = newSequence;
  refreshWaveform();
  update();
}

void SequenceWaveformWidget::refreshWaveform() {
  hasWaveform = false;
  waveformData.clear();

  if (sequence == nullptr || sequence->audioSource == nullptr ||
      sequence->audioSource->reader == nullptr) {
    return;
  }

  try {
    auto *reader = sequence->audioSource->reader;
    if (!reader->IsOpen()) {
      reader->Open();
    }

    openshot::AudioWaveformer waveformer(reader);
    waveformData = waveformer.ExtractSamples(-1, 80, true);
    hasWaveform = !waveformData.max_samples.empty();
  } catch (...) {
    waveformData.clear();
    hasWaveform = false;
  }
}

void SequenceWaveformWidget::paintTimeline(QPainter &painter,
                                          const QRect &plotArea) {
  drawHorizontalGrid(painter, plotArea, 4);
  drawAxisLabels(painter, plotArea, tr("Amp"), tr("Audio"));

  if (sequence == nullptr) {
    drawCenteredMessage(painter, plotArea, tr("Select a sequence"));
    return;
  }

  if (sequence->audioSource == nullptr) {
    drawCenteredMessage(painter, plotArea, tr("Assign an audio source"));
    return;
  }

  if (!hasWaveform || waveformData.max_samples.empty()) {
    drawCenteredMessage(painter, plotArea, tr("Waveform unavailable"));
    return;
  }

  const int centerY = plotArea.center().y();
  const int sampleCount = static_cast<int>(waveformData.max_samples.size());
  if (sampleCount <= 0) {
    drawCenteredMessage(painter, plotArea, tr("Waveform unavailable"));
    return;
  }

  QPainterPath envelope;
  envelope.moveTo(plotArea.left(), centerY);
  for (int i = 0; i < sampleCount; ++i) {
    const double ratio = (sampleCount == 1)
                             ? 0.0
                             : static_cast<double>(i) /
                                   static_cast<double>(sampleCount - 1);
    const int x = plotArea.left() +
                  static_cast<int>(ratio * static_cast<double>(plotArea.width()));
    const double amplitude = std::clamp(
        static_cast<double>(waveformData.max_samples[static_cast<size_t>(i)]),
        0.0, 1.0);
    const int y = centerY -
                  static_cast<int>(amplitude * (plotArea.height() / 2.0 - 4.0));
    envelope.lineTo(x, y);
  }
  for (int i = sampleCount - 1; i >= 0; --i) {
    const double ratio = (sampleCount == 1)
                             ? 0.0
                             : static_cast<double>(i) /
                                   static_cast<double>(sampleCount - 1);
    const int x = plotArea.left() +
                  static_cast<int>(ratio * static_cast<double>(plotArea.width()));
    const double amplitude = std::clamp(
        static_cast<double>(waveformData.max_samples[static_cast<size_t>(i)]),
        0.0, 1.0);
    const int y = centerY +
                  static_cast<int>(amplitude * (plotArea.height() / 2.0 - 4.0));
    envelope.lineTo(x, y);
  }
  envelope.closeSubpath();

  painter.fillPath(envelope, palette().primary);
  painter.setPen(QPen(palette().highlight, 1.5));
  painter.drawLine(plotArea.left(), centerY, plotArea.right(), centerY);
}

} // namespace mechanizm