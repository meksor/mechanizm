#include "gui/source/source_info.h"
#include <QLineEdit>

namespace mechanizm {

SourceInfo::SourceInfo(QWidget *parent) : QWidget(parent) {
  flay = new QFormLayout();
  nameEdit = new QLineEdit();
  nameEdit->setReadOnly(true);

  flay->addRow(tr("Name:"), nameEdit);
  this->setLayout(flay);
}

void SourceInfo::onSourceSelected(mechanizm::Source *s) {
  source = s;
  nameEdit->setText(QString::fromStdString(source->name));
};

} // namespace mechanizm
