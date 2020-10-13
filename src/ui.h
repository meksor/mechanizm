#pragma once

#include "browser.h"
#include "project.h"

namespace mechanizm {
    class ProjectUiController {
    public:
        ProjectUiController() {};
        virtual ~ProjectUiController() {};

        void setProject(mechanizm::Project::shared_ptr p) { project = p; }
        void openBrowserWindow();
    private:
        mechanizm::Project::shared_ptr project;
        std::list<mechanizm::BrowserWindow::shared_ptr> browserWindows;
    };
}