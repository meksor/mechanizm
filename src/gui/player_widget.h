#pragma once

#include <Qt/VideoRenderWidget.h>
#include <QtPlayer.h>
#include <ReaderBase.h>
#include <memory>

#include "clip.h"
#include <QAction>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QShowEvent>
#include <QWidget>

namespace mechanizm {

class PlayerWidget : public QWidget {
  Q_OBJECT
public:
  openshot::QtPlayer *player;
  VideoRenderWidget *renderWidget;

  explicit PlayerWidget(QWidget *parent = 0);

  void createActions();
  QAction *iSpdAct;
  QAction *dSpdAct;
  QAction *iFrmAct;
  QAction *dFrmAct;
  QAction *tpAct;
  QAction *tsAct;

  void setClip(mechanizm::Clip *);
  void onRythmicPointSelected(mechanizm::RythmicPoint);

  void increaseSpeed();
  void decreaseSpeed();
  void increaseFrame();
  void decreaseFrame();
  void togglePause();
  void toggleStop();

protected:
private:
  float playerSpeed = 1.0;
};

} // namespace mechanizm