#pragma once

#include <QApplication>
#include <QString>

#include "launcher_widget.h"
#include "project.h"
#include "sources_widget.h"
#include "sequences_widget.h"
#include "clips_widget.h"
#include "clip_editor_widget.h"
#include "mappings_widget.h"
#include "mapping_editor_widget.h"

namespace mechanizm {

    class Application : public QApplication {
        Q_OBJECT
    public:
        Application(int &argc, char **argv);
        virtual ~Application() {};

        void openProjectDirectory(QString dirPath);
        void newProject(QString name, QString dirPath);
        void setProject(mechanizm::Project::shared_ptr project);
        void openClipEditor(mechanizm::Clip::shared_ptr clip);
        void openMappingEditor(mechanizm::Mapping::shared_ptr mapping);
        void newMappingRequested();
        void showWindows();

    signals:
        void projectChanged(mechanizm::Project::shared_ptr project);
        void newMapping(mechanizm::Clip::shared_ptr clip, mechanizm::Sequence::shared_ptr sequence);

    private:
        void connectProjectSignals();

        mechanizm::LauncherWidget::unique_ptr launcherWindow;
        mechanizm::SourcesWidget::unique_ptr sourcesWindow;
        mechanizm::SequencesWidget::unique_ptr sequencesWindow;
        mechanizm::ClipsWidget::unique_ptr clipsWindow;
        mechanizm::ClipEditorWidget::unique_ptr clipEditorWindow;
        mechanizm::MappingsWidget::unique_ptr mappingsWindow;
        mechanizm::MappingEditorWidget::unique_ptr mappingEditorWindow;

        mechanizm::Project::shared_ptr project;
    };

}