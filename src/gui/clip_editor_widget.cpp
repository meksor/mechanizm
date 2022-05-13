#include "clip_editor_widget.h"
#include <QHeaderView>

namespace mechanizm {
ClipEditorWidget::ClipEditorWidget(QWidget *parent) : QWidget(parent) {
  this->setWindowTitle("Clip Editor");
  hbox = std::make_unique<QHBoxLayout>(this);

  table = std::make_unique<QTableWidget>();
  table->setColumnCount(2);
  table->setHorizontalHeaderItem(0, new QTableWidgetItem("Frame #"));
  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Timestamp"));
  table->setFocusPolicy(Qt::NoFocus);
  table->setEditTriggers(QAbstractItemView::DoubleClicked);
  table->setSelectionBehavior(QAbstractItemView::SelectRows);
  table->setSelectionMode(QAbstractItemView::SingleSelection);
  table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  QObject::connect(table.get(), &QTableWidget::cellClicked, this,
                   &mechanizm::ClipEditorWidget::onCellSelected);

  player = std::make_unique<mechanizm::PlayerWidget>();
  hbox->addWidget(player.get());
  hbox->addWidget(table.get());

  this->setFixedSize(900, 500);
  this->setLayout(hbox.get());
}

void ClipEditorWidget::setClip(mechanizm::Clip::shared_ptr c) {
  clip = c;
  player->setReader(clip->getProxyReader().get());

  player->getPlayer()->Stop();
  player->getPlayer()->Seek(0);
  player->getPlayer()->Play();

  updateTable();
}

void ClipEditorWidget::onCellSelected(int row, int column) {
  auto points = clip->getRythmicPoints();
  auto point = points.GetPoint(row);
  player->getPlayer()->Pause();
  player->getPlayer()->Seek(point.co.X);
}

void ClipEditorWidget::setRythmicPoint() {
  long frame = player->getPlayer()->Position();
  openshot::Fraction fps = clip->getProxyReader()->info.fps;
  float timestamp = frame / fps.ToFloat();
  clip->addRythmicPoint(frame, timestamp);
  updateTable();
}

void ClipEditorWidget::updateTable() {
  table->clearContents();
  auto points = clip->getRythmicPoints();
  int count = points.GetCount();
  table->setRowCount(count);
  for (int i = 0; i < count; i++) {
    auto point = points.GetPoint(i);
    QTableWidgetItem *frameItem =
        new QTableWidgetItem(tr("%1").arg(point.co.X));
    QTableWidgetItem *timestampItem =
        new QTableWidgetItem(tr("%1s").arg(point.co.Y));
    table->setItem(i, 0, frameItem);
    table->setItem(i, 1, timestampItem);
  }
}

void ClipEditorWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_R:
    setRythmicPoint();
    break;
  default:
    player->keyPressEvent(event);
    break;
  }
}
} // namespace mechanizm
