#include "gui/mapping_info.h"
#include <QLineEdit>

namespace mechanizm {

MappingInfo::MappingInfo(QWidget *parent) : QWidget(parent) {
  flay = new QFormLayout();
  nameEdit = new QLineEdit();
  clipEdit = new QLineEdit();
  clipEdit->setReadOnly(true);

  flay->addRow(tr("Name:"), nameEdit);
  flay->addRow(tr("Clip:"), clipEdit);
  this->setLayout(flay);
}

void MappingInfo::onMappingSelected(mechanizm::Mapping *m) {
  mapping = m;
  disconnect(nameEdit, &QLineEdit::textChanged, nullptr, nullptr);
  nameEdit->setText(QString::fromStdString(mapping->name));
  connect(nameEdit, &QLineEdit::textChanged, mapping,
          &mechanizm::Mapping::setName);
  clipEdit->setText(QString::fromStdString(mapping->clip->name));

};

} // namespace mechanizm
