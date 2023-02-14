#include "gui/sequence_info.h"
#include "sequence.h"
#include <QLineEdit>

namespace mechanizm {

SequenceInfo::SequenceInfo(QWidget *parent) : QWidget(parent) {
  flay = new QFormLayout();
  nameEdit = new QLineEdit();

  flay->addRow(tr("Name:"), nameEdit);
  this->setLayout(flay);
}

void SequenceInfo::onSequenceSelected(mechanizm::Sequence *s) {
  disconnect(nameEdit, &QLineEdit::textChanged, nullptr, nullptr);
  sequence = s;
  nameEdit->setText(QString::fromStdString(sequence->name));
  connect(nameEdit, &QLineEdit::textChanged, sequence,
          &mechanizm::Sequence::setName);
};

} // namespace mechanizm
