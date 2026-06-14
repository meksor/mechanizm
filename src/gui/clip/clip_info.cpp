#include "gui/clip/clip_info.h"
#include "clip.h"
#include <QLineEdit>

namespace mechanizm {

ClipInfo::ClipInfo(QWidget *parent) : QWidget(parent) {
  flay = new QFormLayout();
  nameEdit = new QLineEdit();
  startFrameEdit = new QSpinBox();
  startFrameEdit->setReadOnly(true);
  endFrameEdit = new QSpinBox();
  endFrameEdit->setReadOnly(true);

  flay->addRow(tr("Name:"), nameEdit);
  flay->addRow(tr("Start Frame:"), startFrameEdit);
  flay->addRow(tr("End Frame:"), endFrameEdit);
  this->setLayout(flay);
}

void ClipInfo::onClipSelected(mechanizm::Clip *c) {
  disconnect(nameEdit, &QLineEdit::textChanged, nullptr, nullptr);
  clip = c;
  renderClip(clip);
  if (clip != nullptr) {
    connect(nameEdit, &QLineEdit::textChanged, clip, &mechanizm::Clip::setName);
  }
};

void ClipInfo::renderClip(mechanizm::Clip *c) {
  if (c == nullptr || c->source == nullptr || c->source->reader == nullptr) {
    nameEdit->clear();
    startFrameEdit->setValue(0);
    endFrameEdit->setValue(0);
    return;
  }

  nameEdit->setText(QString::fromStdString(c->name));
  startFrameEdit->setMaximum(c->source->reader->info.video_length);
  endFrameEdit->setMaximum(c->source->reader->info.video_length);
  startFrameEdit->setValue(static_cast<int>(c->getFirstFrame()));
  endFrameEdit->setValue(static_cast<int>(c->getLastFrame()));
}

} // namespace mechanizm
