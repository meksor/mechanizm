#pragma once

#include "project.h"
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QWidget>

namespace mechanizm {

class SourceInfo : public QWidget {
  Q_OBJECT
public:
  SourceInfo(QWidget *parent = nullptr);

  void onSourceSelected(mechanizm::Source *s);

signals:
private:
  mechanizm::Source *source;

  QFormLayout *flay;
  QLineEdit *nameEdit;
};

} // namespace mechanizm