#include <memory>
#include <QObject>
#include <QStyleFactory>

#include "application.h"

namespace mechanizm {
    
    Application::Application(int &argc, char **argv) 
            : QApplication(argc, argv) {
        this->setOrganizationName("mircosoft");
        this->setOrganizationDomain("mircosoft.net");
        this->setApplicationName("mechanizm");

        launcherWindow = std::make_unique<mechanizm::LauncherWidget>();
        clipsWindow = std::make_unique<mechanizm::ClipsWidget>();
        editorWindow = std::make_unique<mechanizm::EditorWidget>();
        sourcesWindow = std::make_unique<mechanizm::SourcesWidget>();

        QObject::connect(launcherWindow.get(), &mechanizm::LauncherWidget::newProject, 
                this, &Application::newProject);
        QObject::connect(launcherWindow.get(), &mechanizm::LauncherWidget::openProjectDirectory, 
                this, &Application::openProjectDirectory);
        QObject::connect(clipsWindow.get(), &mechanizm::ClipsWidget::clipEditorRequested,
            this, &mechanizm::Application::openClipEditor);

        launcherWindow->show();
        clipsWindow->show();
        sourcesWindow->show();
    }

    void Application::openProjectDirectory(QString dirPath) {
        setProject(std::make_shared<mechanizm::Project>(dirPath));
        connectProjectSignals();
        project->loadFromDisk();
    }

    void Application::newProject(QString name, QString dirPath) {
        setProject(std::make_shared<mechanizm::Project>(dirPath, name));
        connectProjectSignals();
        project->loadFromDisk();
    }

    void Application::openClipEditor(mechanizm::Clip::shared_ptr c) {
        editorWindow->setClip(c);
        editorWindow->show();
    }   

    void Application::connectProjectSignals() {
        QObject::connect(project.get(), &mechanizm::Project::sourcesChanged,
            sourcesWindow.get(), &mechanizm::SourcesWidget::setSources);

        QObject::connect(sourcesWindow.get(), &mechanizm::SourcesWidget::importFileSelected,
            project.get(), &mechanizm::Project::importFile);

        QObject::connect(sourcesWindow.get(), &mechanizm::SourcesWidget::newClip,
            project.get(), &mechanizm::Project::newClip);

        QObject::connect(project.get(), &mechanizm::Project::clipsChanged,
            clipsWindow.get(), &mechanizm::ClipsWidget::setClips);
    }

    void Application::setProject(mechanizm::Project::shared_ptr p) {
        project = p;
        emit projectChanged(project);
    }

}