#include <QFileDialog>

#include "sequences_widget.h"

namespace mechanizm {

SequencesWidget::SequencesWidget(QWidget *parent) : QWidget(parent) {
  this->setWindowTitle("Sequences");
  hbox = std::make_unique<QHBoxLayout>(this);
  list = std::make_unique<QListWidget>();
  list->setFocusPolicy(Qt::NoFocus);

  this->setFixedSize(480, 480);

  hbox->addWidget(list.get());
  setLayout(hbox.get());
}

void SequencesWidget::setSequences(
    std::vector<mechanizm::Sequence::shared_ptr> s) {
  sequences = s;
  updateList();
}

mechanizm::Sequence::shared_ptr SequencesWidget::currentSequence() {
  return sequences[list->currentRow()];
}

void SequencesWidget::updateList() {
  list->clear();
  for (auto s : sequences) {
    list->addItem(s->getName());
  }
}

void SequencesWidget::openImportDialogue() {
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilter(tr("MIDI Files (*.mid)"));
  int result = dialog.exec();
  QString filePath;
  mechanizm::Sequence::shared_ptr sequence;
  if (result) {
    filePath = dialog.selectedFiles()[0];
    emit importFileSelected(filePath);
  }
}

void SequencesWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_I:
    this->openImportDialogue();
    break;
  case Qt::Key_M:
    emit newMapping();
    break;
  default:
    QWidget::keyPressEvent(event);
    break;
  }
}

} // namespace mechanizm