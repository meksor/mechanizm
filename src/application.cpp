#include <QDir>
#include <QObject>
#include <QStyleFactory>
#include <memory>

#include "application.h"

namespace mechanizm {

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {
  this->setOrganizationName("mircosoft");
  this->setOrganizationDomain("mircosoft.net");
  this->setApplicationName("mechanizm");

  launcherWindow = std::make_unique<mechanizm::LauncherWidget>();
  sourcesWindow = std::make_unique<mechanizm::SourcesWidget>();
  sequencesWindow = std::make_unique<mechanizm::SequencesWidget>();
  clipsWindow = std::make_unique<mechanizm::ClipsWidget>();
  mappingsWindow = std::make_unique<mechanizm::MappingsWidget>();

  QObject::connect(launcherWindow.get(), &mechanizm::LauncherWidget::newProject,
                   this, &Application::newProject);
  QObject::connect(launcherWindow.get(),
                   &mechanizm::LauncherWidget::openProjectDirectory, this,
                   &Application::openProjectDirectory);

  QObject::connect(clipsWindow.get(),
                   &mechanizm::ClipsWidget::clipEditorRequested, this,
                   &mechanizm::Application::openClipEditor);
  QObject::connect(mappingsWindow.get(),
                   &mechanizm::MappingsWidget::mappingEditorRequested, this,
                   &mechanizm::Application::openMappingEditor);

  launcherWindow->show();
}

void Application::openProjectDirectory(QString dirPath) {
  QDir projectDir(dirPath);
  setProject(std::make_shared<mechanizm::Project>(projectDir));
  connectProjectSignals();
  showWindows();
}

void Application::newProject(QString name, QString dirPath) {
  QDir projectDir(dirPath);
  setProject(std::make_shared<mechanizm::Project>(projectDir, name));
  connectProjectSignals();
  showWindows();
}

void Application::showWindows() {
  clipsWindow->show();
  sourcesWindow->show();
  sequencesWindow->show();
  mappingsWindow->show();
  launcherWindow->close();
}

void Application::openClipEditor(mechanizm::Clip::shared_ptr clip) {
  clipEditorWindow->setClip(clip);
  clipEditorWindow->show();
}

void Application::openMappingEditor(mechanizm::Mapping::shared_ptr mapping) {
  mappingEditorWindow->setMapping(mapping);
  mappingEditorWindow->show();
}

void Application::connectProjectSignals() {
  QObject::connect(project.get(), &mechanizm::Project::sourcesChanged,
                   sourcesWindow.get(), &mechanizm::SourcesWidget::setSources);

  QObject::connect(sourcesWindow.get(),
                   &mechanizm::SourcesWidget::importFileSelected, project.get(),
                   &mechanizm::Project::importSourceFile);

  QObject::connect(project.get(), &mechanizm::Project::sequencesChanged,
                   sequencesWindow.get(),
                   &mechanizm::SequencesWidget::setSequences);

  QObject::connect(sequencesWindow.get(),
                   &mechanizm::SequencesWidget::importFileSelected,
                   project.get(), &mechanizm::Project::importSequenceFile);

  QObject::connect(project.get(), &mechanizm::Project::clipsChanged,
                   clipsWindow.get(), &mechanizm::ClipsWidget::setClips);

  QObject::connect(sourcesWindow.get(), &mechanizm::SourcesWidget::newClip,
                   project.get(), &mechanizm::Project::newClip);

  QObject::connect(project.get(), &mechanizm::Project::mappingsChanged,
                   mappingsWindow.get(),
                   &mechanizm::MappingsWidget::setMappings);

  QObject::connect(sequencesWindow.get(),
                   &mechanizm::SequencesWidget::newMapping, this,
                   &Application::newMappingRequested);

  QObject::connect(this, &Application::newMapping, project.get(),
                   &mechanizm::Project::newMapping);

  project->emitAllSignals();
}

void Application::newMappingRequested() {
  emit newMapping(clipsWindow->currentClip(),
                  sequencesWindow->currentSequence());
}

void Application::setProject(mechanizm::Project::shared_ptr p) {
  project = p;
  emit projectChanged(project);
}

} // namespace mechanizm