#include "gui/sequence_info.h"
#include <QLineEdit>

namespace mechanizm {

SequenceInfo::SequenceInfo(QWidget *parent) : QWidget(parent) {
  flay = new QFormLayout();
  nameEdit = new QLineEdit();
  nameEdit->setReadOnly(true);

  flay->addRow(tr("Name:"), nameEdit);
  this->setLayout(flay);
}

void SequenceInfo::onSequenceSelected(mechanizm::Sequence *s) {
  sequence = s;
  nameEdit->setText(QString::fromStdString(sequence->name));
};

} // namespace mechanizm
