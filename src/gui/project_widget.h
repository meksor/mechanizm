#pragma once

#include "project.h"
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QWidget>

namespace mechanizm {

class ProjectWidget : public QWidget {
  Q_OBJECT
public:
  ProjectWidget(QWidget *parent = nullptr);

  void onProjectChanged(mechanizm::Project *p);

signals:
private:
  QFormLayout *flay;
  QLineEdit *nameEdit;
  QLineEdit *pathEdit;
  QDoubleSpinBox *bpmEdit;
  // TODO: time div
};

} // namespace mechanizm