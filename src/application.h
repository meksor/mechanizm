#pragma once

#include <QApplication>
#include <QString>
#include <QStyle>

#include "gui/main_window.h"

namespace mechanizm {

class Application : public QApplication {
  Q_OBJECT
public:
  Application(int &argc, char **argv);
  virtual ~Application(){};

private:
  mechanizm::MainWindow *mainWindow;
};

} // namespace mechanizm