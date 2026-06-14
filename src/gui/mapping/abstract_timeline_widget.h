#pragma once

#include <QColor>
#include <QWidget>

class QPainter;
class QRect;

namespace mechanizm {

class AbstractTimelineWidget : public QWidget {
  Q_OBJECT
public:
  explicit AbstractTimelineWidget(QWidget *parent = nullptr);

protected:
  struct PlotMargins {
    int left = 48;
    int right = 16;
    int top = 16;
    int bottom = 28;
  };

  struct TimelineColors {
    QColor background = QColor(20, 24, 30);
    QColor border = QColor(54, 62, 74);
    QColor grid = QColor(70, 80, 96);
    QColor text = QColor(180, 185, 194);
    QColor primary = QColor(102, 193, 255);
    QColor accent = QColor(255, 199, 95);
    QColor cursor = QColor(255, 184, 76);
    QColor highlight = QColor(122, 214, 170);
    QColor outsideRegion = QColor(56, 62, 74, 130);
  };

  const TimelineColors &palette() const { return colors; }
  QRect buildPlotArea(const PlotMargins &margins) const;

  void drawPlotFrame(QPainter &painter, const QRect &plotArea) const;
  void drawHorizontalGrid(QPainter &painter, const QRect &plotArea,
                          int segments) const;
  void drawAxisLabels(QPainter &painter, const QRect &plotArea,
                      const QString &leftLabel,
                      const QString &bottomLabel) const;
  void drawCenteredMessage(QPainter &painter, const QRect &plotArea,
                           const QString &message) const;

  virtual void paintTimeline(QPainter &painter, const QRect &plotArea) = 0;

  void paintEvent(QPaintEvent *event) override;

private:
  TimelineColors colors;
};

} // namespace mechanizm
