#include "gui/project_widget.h"
#include "project.h"
#include <qlineedit.h>

namespace mechanizm {

ProjectWidget::ProjectWidget(QWidget *parent) : QWidget(parent) {
  flay = new QFormLayout();
  nameEdit = new QLineEdit();
  pathEdit = new QLineEdit();
  pathEdit->setReadOnly(true);

  bpmEdit = new QDoubleSpinBox();
  bpmEdit->setMinimum(0.);
  bpmEdit->setMaximum(300.);

  flay->addRow(tr("Name:"), nameEdit);
  flay->addRow(tr("Path:"), pathEdit);
  flay->addRow(tr("BPM:"), bpmEdit);

  setLayout(flay);
}

void ProjectWidget::onProjectChanged(mechanizm::Project *p) {
  nameEdit->setText(QString::fromStdString(p->name));
  pathEdit->setText(p->getPath());
  bpmEdit->setValue(p->bpm);

  disconnect(nameEdit, &QLineEdit::textChanged, nullptr, nullptr);
  connect(nameEdit, &QLineEdit::textChanged, p, &mechanizm::Project::setQName);
  // weird qt fuckyness.
  disconnect(bpmEdit, qOverload<double>(&QDoubleSpinBox::valueChanged), nullptr,
             nullptr);
  connect(bpmEdit, qOverload<double>(&QDoubleSpinBox::valueChanged), p,
          &mechanizm::Project::setBpm);
}

} // namespace mechanizm
