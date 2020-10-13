#include <memory>
#include "ui.h"

namespace mechanizm {
    void ProjectUiController::openBrowserWindow() {
        auto window = std::make_shared<mechanizm::BrowserWindow>();
        window->setProject(project);
        window->show();
        browserWindows.push_back(window);
    }
}