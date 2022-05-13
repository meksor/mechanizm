#include "clips_widget.h"

namespace mechanizm {
ClipsWidget::ClipsWidget(QWidget *parent) : QWidget(parent) {
  this->setWindowTitle("Clips");
  hbox = std::make_unique<QHBoxLayout>(this);
  list = std::make_unique<QListWidget>();
  list->setFocusPolicy(Qt::NoFocus);

  this->setFixedSize(480, 480);
  hbox->addWidget(list.get());
  setLayout(hbox.get());
}

void ClipsWidget::setClips(std::vector<mechanizm::Clip::shared_ptr> c) {
  clips = c;
  updateList();
};

void ClipsWidget::updateList() {
  list->clear();
  for (auto clip : clips) {
    QString name = clip->getName();
    list->addItem(name);
  }
}

mechanizm::Clip::shared_ptr ClipsWidget::currentClip() {
  return clips[list->currentRow()];
}

void ClipsWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_E:
    event->accept();
    emit clipEditorRequested(currentClip());
    break;
  default:
    QWidget::keyPressEvent(event);
    break;
  }
}

} // namespace mechanizm
