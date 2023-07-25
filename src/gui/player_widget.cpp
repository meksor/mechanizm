#include <OpenShot.h>

#include "FFmpegReader.h"
#include "PlayerBase.h"
#include "Timeline.h"
#include "gui/player_widget.h"

namespace mechanizm {

PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent) {
  renderWidget = new VideoRenderWidget(this);
  player = new openshot::QtPlayer(renderWidget->GetRenderer());
  createActions(); // playerMenu->addAction(player->iSpdAct);
  this->addAction(dSpdAct);
  this->addAction(iFrmAct);
  this->addAction(dFrmAct);
  this->addAction(tpAct);
}

void PlayerWidget::createActions() {
  iSpdAct = new QAction(tr("&Increase Speed"), this);
  iSpdAct->setShortcut(Qt::Key_L);
  iSpdAct->setStatusTip(tr("Increase player speed by x1"));

  dSpdAct = new QAction(tr("&Decrease Speed"), this);
  dSpdAct->setShortcut(Qt::Key_J);
  dSpdAct->setStatusTip(tr("Decrease player speed by x1"));

  iFrmAct = new QAction(tr("&Increase Frame"), this);
  iFrmAct->setShortcut(Qt::Key_Right);
  iFrmAct->setStatusTip(tr("Step one frame forward"));

  dFrmAct = new QAction(tr("&Decrease Frame"), this);
  dFrmAct->setShortcut(Qt::Key_Left);
  dFrmAct->setStatusTip(tr("Step one frame backward"));

  tpAct = new QAction(tr("&Toggle Pause"), this);
  tpAct->setShortcut(Qt::Key_K);
  tpAct->setStatusTip(tr("Toggle paused state"));

  tsAct = new QAction(tr("&Toggle Stop"), this);
  tsAct->setShortcut(Qt::Key_Space);
  tsAct->setStatusTip(tr("Toggle stopped state"));

  connect(iSpdAct, &QAction::triggered, this, &PlayerWidget::increaseSpeed);
  connect(dSpdAct, &QAction::triggered, this, &PlayerWidget::decreaseSpeed);
  connect(iFrmAct, &QAction::triggered, this, &PlayerWidget::increaseFrame);
  connect(dFrmAct, &QAction::triggered, this, &PlayerWidget::decreaseFrame);
  connect(tpAct, &QAction::triggered, this, &PlayerWidget::togglePause);
  connect(tsAct, &QAction::triggered, this, &PlayerWidget::toggleStop);
}

void PlayerWidget::increaseSpeed() {
  playerSpeed += 1.;
  player->Speed(playerSpeed);
  if (player->Mode() != openshot::PLAYBACK_PLAY)
    player->Play();
}

void PlayerWidget::decreaseSpeed() {
  playerSpeed -= 1.;
  player->Speed(playerSpeed);
  if (player->Mode() != openshot::PLAYBACK_PLAY)
    player->Play();
}

void PlayerWidget::increaseFrame() {
  playerSpeed = 0.;
  player->Speed(playerSpeed);
  player->Seek(player->Position() + 1);
}

void PlayerWidget::decreaseFrame() {
  playerSpeed = 0.;
  player->Speed(playerSpeed);
  player->Seek(player->Position() - 1);
}

void PlayerWidget::togglePause() {
  if (playerSpeed == 0.) {
    playerSpeed = 1.;
  } else {
    playerSpeed = 0.;
  }
  player->Speed(playerSpeed);
}

void PlayerWidget::toggleStop() {
  if (player->Mode() == openshot::PLAYBACK_PLAY) {
    player->Pause();
  } else {
    player->Play();
  }
}

void PlayerWidget::setClip(mechanizm::Clip *clip) {
  player->Stop();
  player->SetSource(clip->source->path);

  // this->setFixedSize(r->info.width, r->info.height);
  // renderWidget->setFixedSize(r->info.width, r->info.height);
  player->Play();
  player->Seek(clip->getFirstFrame());
  playerSpeed = 1.;
  player->Speed(playerSpeed);
  player->Pause();
}

void PlayerWidget::setReader(openshot::ReaderBase *r) {
  if (!r->IsOpen())
    r->Open();
  player->Reader(r);
  player->Seek(0);
  playerSpeed = 1.;
  player->Speed(playerSpeed);
  player->Pause();
}

void PlayerWidget::onRythmicPointSelected(mechanizm::RythmicPoint rp) {
  player->Seek(rp.frame);
};

} // namespace mechanizm