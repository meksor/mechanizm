#include "clip_editor_window.h"
#include "clip.h"
#include <QHeaderView>

#include "project.h"

namespace mechanizm {
ClipEditorWindow::ClipEditorWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags) {
  this->setWindowTitle("Clip Editor");
  createActions();

  QWidget *widget = new QWidget;
  hbox = new QHBoxLayout(widget);

  leftColumn = new QVBoxLayout();
  clipTable = new mechanizm::ClipTable();
  clipInfo = new mechanizm::ClipInfo();
  clipInfo->setFixedWidth(300);
  leftColumn->addWidget(clipTable);
  leftColumn->addWidget(clipInfo);
  hbox->addLayout(leftColumn);

  rpTable = new mechanizm::RythmicPointTable();
  rpTable->setFixedWidth(250);
  hbox->addWidget(rpTable);

  player = new mechanizm::PlayerWidget();
  player->setFixedSize(720, 480);
  player->renderWidget->setFixedSize(720, 480);

  timeline = new mechanizm::ClipTimeline();
  timeline->setFixedHeight(130);

  previewColumn = new QVBoxLayout();
  previewColumn->addWidget(player);
  previewColumn->addWidget(timeline);
  hbox->addLayout(previewColumn);

  cursorSyncTimer = new QTimer(this);
  cursorSyncTimer->setInterval(33);
  connect(cursorSyncTimer, &QTimer::timeout, this, [this]() {
    if (timeline == nullptr || player == nullptr || player->player == nullptr) {
      return;
    }
    timeline->setCursorFrame(player->player->Position());
  });
  cursorSyncTimer->start();

  createMenus();

  connect(rpTable, &mechanizm::RythmicPointTable::selectRythmicPoint, player,
          &mechanizm::PlayerWidget::onRythmicPointSelected);
    connect(clipTable, &mechanizm::ClipTable::selectClip, this,
      &ClipEditorWindow::onClipSelected);
    connect(clipTable, &mechanizm::ClipTable::selectClip, clipInfo,
      &ClipInfo::onClipSelected);

  setCentralWidget(widget);
}

  void ClipEditorWindow::onProjectChanged(mechanizm::Project *p) {
    project = p;
    connect(project, &Project::clipsChanged, clipTable,
      &mechanizm::ClipTable::onClipsChanged);
  }

void ClipEditorWindow::createActions() {
  removeAct = new QAction(tr("&Remove Rythmic Point"), this);
  removeAct->setShortcuts(QKeySequence::Delete);
  removeAct->setStatusTip(tr("Remove the selected rythmic point"));

  addAct = new QAction(tr("&Add Rythmic Point"), this);
  addAct->setShortcut(Qt::Key_R);
  addAct->setStatusTip(tr("Add a new rythmic point"));

  connect(removeAct, &QAction::triggered, this,
          &ClipEditorWindow::removeSelectedRythmicPoint);
  connect(addAct, &QAction::triggered, this,
          &ClipEditorWindow::addRythmicPoint);
}

void ClipEditorWindow::createMenus() {
  rpMenu = this->menuBar()->addMenu(tr("&Rythmic Points"));
  rpMenu->addAction(removeAct);
  rpMenu->addAction(addAct);
  playerMenu = this->menuBar()->addMenu(tr("&Player"));
  playerMenu->addAction(player->iSpdAct);
  playerMenu->addAction(player->dSpdAct);
  playerMenu->addAction(player->tpAct);
  playerMenu->addAction(player->tsAct);
  playerMenu->addSeparator();
  playerMenu->addAction(player->iFrmAct);
  playerMenu->addAction(player->dFrmAct);
}
void ClipEditorWindow::onClipSelected(mechanizm::Clip *c) {
  if (c == nullptr) {
    clip = nullptr;
    if (timeline != nullptr) {
      timeline->onClipSelected(nullptr);
    }
    return;
  }
  clip = c;
  clipInfo->onClipSelected(clip);
  player->setClip(clip);
  rpTable->onClipUpdated(clip);
  if (timeline != nullptr) {
    timeline->onClipSelected(clip);
  }
}
void ClipEditorWindow::removeSelectedRythmicPoint() {
  if (clip == nullptr || clip->rythmicPoints.empty()) {
    return;
  }
  mechanizm::RythmicPoint rp = rpTable->getSelectedRythmicPoint();
  clip->removeRythmicPoint(rp);
  rpTable->onClipUpdated(clip);
};
void ClipEditorWindow::addRythmicPoint() {
  if (clip == nullptr) {
    return;
  }
  mechanizm::id_t id = mechanizm::RythmicPoint::getNextId(clip->rythmicPoints);
  mechanizm::RythmicPoint rp(id, player->player->Position());
  clip->addRythmicPoint(rp);
  rpTable->onClipUpdated(clip);
};

} // namespace mechanizm