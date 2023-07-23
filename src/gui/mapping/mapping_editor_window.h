
#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

#include "channel_table.h"
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
  void onProjectChanged(mechanizm::Project *p) { project = p; };

  void createMenus();
  void createActions();

  void removeSelectedChannel();
  void renderPreview();
  void addChannel();

protected:
private:
  void updateTable();

  QHBoxLayout *hbox;
  QAction *previewAct;
  QAction *removeAct;
  QAction *addAct;
  QMenu *compMenu;
  QMenu *chMenu;
  QMenu *playerMenu;
  mechanizm::Channel *channel;
  mechanizm::Sequence *sequence;
  mechanizm::ChannelTable *chTable;
  mechanizm::PlayerWidget *player;
  mechanizm::Project *project;
  mechanizm::Mapping *mapping;
};
} // namespace mechanizm