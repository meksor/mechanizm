
#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QProgressBar>
#include <QComboBox>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>
#include <QString>
#include <cstddef>
#include <libopenshot/Timeline.h>

#include "channel_table.h"
#include "mapping_timeline.h"
#include "gui/player_widget.h"
#include "mapping.h"
#include "project.h"

namespace mechanizm {

class MappingEditorWindow : public QMainWindow {
  Q_OBJECT
public:
  MappingEditorWindow(QWidget *parent = nullptr,
                      Qt::WindowFlags flags = Qt::WindowFlags());

  void onChannelSelected(mechanizm::Channel *);
  void onSequenceSelected(mechanizm::Sequence *s) { sequence = s; };
  void onMappingSelected(mechanizm::Mapping *m);
  void onProjectChanged(mechanizm::Project *p) {
    project = p;
    if (timeline != nullptr && project != nullptr) {
      timeline->setBpm(project->bpm);
    }
    renderPreview();
  };
  void createMenus();
  void createActions();

  void removeSelectedChannel();
  void renderPreview();
  void onRenderPreviewRequested();
  void addChannel();
  void onChannelEffectChanged(int effect);
  void onChannelInterpolationChanged(int interpolation);
  void onChannelValueChanged(int value);
  void onWrapBehaviourChanged(int wrapBehaviour);

protected:
private:
  void updateTable();
  void disconnectMappingSignals();
  void schedulePreviewRender();
  void updateTimelineCursor();
  void refreshChannelEditor();
  void updateRenderStatusUi(bool running, int progress,
                            const QString &message);
  QString defaultPreviewPath() const;
  bool renderPreviewToFile(const QString &filePath);

  QHBoxLayout *hbox;
  QVBoxLayout *leftPanel;
  QVBoxLayout *rightPanel;
  QAction *previewAct;
  QAction *removeAct;
  QAction *addAct;
  QMenu *compMenu;
  QMenu *chMenu;
  QMenu *playerMenu;
  QComboBox *effectEdit;
  QComboBox *interpolationEdit;
  QSpinBox *valueEdit;
  QComboBox *wrapBehaviourEdit;
  int selectedChannelRow = -1;
  mechanizm::Sequence *sequence = nullptr;
  mechanizm::ChannelTable *chTable;
  mechanizm::MappingTimeline *timeline;
  mechanizm::PlayerWidget *player;
  mechanizm::Project *project = nullptr;
  mechanizm::Mapping *mapping = nullptr;
  openshot::Timeline *previewTimeline = nullptr;
  QString previewOutputPath;
  QLabel *renderStatusLabel = nullptr;
  QProgressBar *renderProgressBar = nullptr;
  bool isRenderingPreview = false;
  QTimer *previewRefreshTimer = nullptr;
  QTimer *cursorSyncTimer = nullptr;
};
} // namespace mechanizm