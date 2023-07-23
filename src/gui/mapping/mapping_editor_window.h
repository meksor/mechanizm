
#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>

#include "mapping.h"
#include "gui/player_widget.h"
#include "channel_table.h"

namespace mechanizm {

class MappingEditorWindow : public QMainWindow {
  Q_OBJECT
public:
  MappingEditorWindow(QWidget *parent = nullptr,
                   Qt::WindowFlags flags = Qt::WindowFlags());

  void onChannelSelected(mechanizm::Channel *);
  void onSequenceSelected(mechanizm::Sequence *s) {
    sequence = s;
  };
  void onMappingSelected(mechanizm::Mapping *m);


  void createMenus();
  void createActions();

  void removeSelectedChannel();
  void addChannel();

protected:
private:
  void updateTable();

  QHBoxLayout *hbox;
  QAction *removeAct;
  QAction *addAct;
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