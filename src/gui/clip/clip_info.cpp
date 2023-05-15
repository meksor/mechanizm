#include "gui/clip/clip_info.h"
#include "clip.h"
#include <QLineEdit>

namespace mechanizm {

ClipInfo::ClipInfo(QWidget *parent) : QWidget(parent) {
  flay = new QFormLayout();
  nameEdit = new QLineEdit();
  inFrameEdit = new QSpinBox();
  inFrameEdit->setReadOnly(true);
  outFrameEdit = new QSpinBox();
  outFrameEdit->setReadOnly(true);

  flay->addRow(tr("Name:"), nameEdit);
  flay->addRow(tr("In Frame:"), inFrameEdit);
  flay->addRow(tr("Out Frame:"), outFrameEdit);
  this->setLayout(flay);
}

void ClipInfo::onClipSelected(mechanizm::Clip *c) {
  disconnect(nameEdit, &QLineEdit::textChanged, nullptr, nullptr);
  clip = c;
  renderClip(clip);
  connect(nameEdit, &QLineEdit::textChanged, clip, &mechanizm::Clip::setName);
};

void ClipInfo::renderClip(mechanizm::Clip *c) {
  nameEdit->setText(QString::fromStdString(clip->name));
  inFrameEdit->setMaximum(clip->source->reader->info.video_length);
  outFrameEdit->setMaximum(clip->source->reader->info.video_length);
  inFrameEdit->setValue(clip->inFrame);
  outFrameEdit->setValue(clip->outFrame);
}

} // namespace mechanizm
