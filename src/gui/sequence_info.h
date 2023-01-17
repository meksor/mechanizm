#pragma once

#include "project.h"
#include "sequence.h"
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QWidget>

namespace mechanizm {

class SequenceInfo : public QWidget {
  Q_OBJECT
public:
  SequenceInfo(QWidget *parent = nullptr);

  void onSequenceSelected(mechanizm::Sequence *);

signals:
private:
  mechanizm::Sequence *sequence;

  QFormLayout *flay;
  QLineEdit *nameEdit;
};

} // namespace mechanizm