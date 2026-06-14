
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
#include <memory>
#include <vector>

#include "channel_table.h"
#include "clip_timeline.h"
#include "gui/mapping/mapping_info.h"
#include "gui/mapping/mapping_table.h"
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
  void onProjectChanged(mechanizm::Project *p);
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
  QVBoxLayout *overviewPanel;
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
  mechanizm::MappingTable *mappingTable;
  mechanizm::MappingInfo *mappingInfo;
  mechanizm::ChannelTable *chTable;
  mechanizm::MappingTimeline *timeline;
  mechanizm::ClipTimeline *clipTimeline;
  mechanizm::PlayerWidget *player;
  mechanizm::Project *project = nullptr;
  mechanizm::Mapping *mapping = nullptr;
  openshot::Clip *previewClip = nullptr;
  openshot::Timeline *previewTimeline = nullptr;
  std::vector<std::unique_ptr<openshot::Clip>> previewAudioClips;
  QString previewOutputPath;
  QLabel *renderStatusLabel = nullptr;
  QProgressBar *renderProgressBar = nullptr;
  bool isRenderingPreview = false;
  QTimer *previewRefreshTimer = nullptr;
  QTimer *cursorSyncTimer = nullptr;
};
} // namespace mechanizm