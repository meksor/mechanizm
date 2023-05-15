#pragma once

#include "project.h"
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QWidget>

namespace mechanizm {

class MappingInfo : public QWidget {
  Q_OBJECT
public:
  MappingInfo(QWidget *parent = nullptr);

  void onMappingSelected(mechanizm::Mapping *m);

signals:
private:
  mechanizm::Mapping *mapping;

  QFormLayout *flay;
  QLineEdit *nameEdit;
  QLineEdit *clipEdit;
};

} // namespace mechanizm