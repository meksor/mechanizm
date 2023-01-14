#include "application.h"
#include "gui/main_window.h"
#include <QCommonStyle>

namespace mechanizm {

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {
  this->setOrganizationName("ungut");
  this->setOrganizationDomain("ungut.at");
  this->setApplicationName("mechanizm");

  mainWindow = new mechanizm::MainWindow();
  mainWindow->show();
}

} // namespace mechanizm