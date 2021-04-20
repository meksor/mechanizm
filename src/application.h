#pragma once

#include <QApplication>
#include <QString>
#include "launcher_widget.h"
#include "project.h"
#include "sources_widget.h"
#include "clips_widget.h"
#include "editor_widget.h"
#include "clip.h"

namespace mechanizm {

    class Application : public QApplication {
        Q_OBJECT
    public:
        Application(int &argc, char **argv);
        virtual ~Application() {};

        void openProjectDirectory(QString dirPath);
        void newProject(QString name, QString dirPath);
        void setProject(mechanizm::Project::shared_ptr p);
        void openClipEditor(mechanizm::Clip::shared_ptr c);

    signals:
        void projectChanged(mechanizm::Project::shared_ptr);

    private:
        void connectProjectSignals();

        mechanizm::LauncherWidget::unique_ptr launcherWindow;
        mechanizm::SourcesWidget::unique_ptr sourcesWindow;
        mechanizm::ClipsWidget::unique_ptr clipsWindow;
        mechanizm::EditorWidget::unique_ptr editorWindow;

        mechanizm::Project::shared_ptr project;
    };

}