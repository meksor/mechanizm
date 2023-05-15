#pragma once

#include "clip.h"
#include "project.h"
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QWidget>
#include <qspinbox.h>

namespace mechanizm {

class ClipInfo : public QWidget {
  Q_OBJECT
public:
  ClipInfo(QWidget *parent = nullptr);

  void renderClip(mechanizm::Clip *c);
  void onClipSelected(mechanizm::Clip *);

signals:
private:
  mechanizm::Clip *clip;

  QFormLayout *flay;
  QLineEdit *nameEdit;
  QSpinBox *inFrameEdit;
  QSpinBox *outFrameEdit;
};

} // namespace mechanizm