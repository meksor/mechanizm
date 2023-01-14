#pragma once
#include "clip.h"
#include "json.h"
#include "mapping.h"
#include "sequence.h"
#include "source.h"
#include <QDir>
#include <QObject>
#include <QString>
#include <memory>
#include <vector>

namespace mechanizm {
class Project : public QObject, public mechanizm::JsonStorable {
  Q_OBJECT
public:
  typedef std::shared_ptr<Project> shared_ptr;

  Project(QDir &rootDir);
  Project(QDir &rootDir, std::string name);

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  virtual QString getDirectoryPath() const override { return "."; };
  virtual QString getJsonFileName() const override { return "project.json"; };
  virtual void setupDirectory() override;

  void setBpm(double b) { bpm = b; };
  void setName(std::string n) { name = n; };
  void setQName(QString n) { setName(n.toStdString()); };
  QString getPath() const { return this->rootDir.absolutePath(); };

  void loadSource(Json::Value);
  void addSource(mechanizm::Source *);
  void removeSource(mechanizm::Source *);

  void loadClip(Json::Value);
  void loadSequence(Json::Value);
  void loadMapping(Json::Value);
  void setDefaults();

  void emitAll() {
    emit sourcesChanged(sources);
    emit clipsChanged(clips);
    emit sequencesChanged(sequences);
    emit mappingsChanged(mappings);
  };

  std::string name;
  double bpm;
  std::pair<int, int> timeDivision;

  std::vector<mechanizm::Source *> sources;
  std::vector<mechanizm::Clip *> clips;
  std::vector<mechanizm::Sequence *> sequences;
  std::vector<mechanizm::Mapping *> mappings;
signals:
  void sourcesChanged(std::vector<mechanizm::Source *> sources);
  void clipsChanged(std::vector<mechanizm::Clip *> clips);
  void sequencesChanged(std::vector<mechanizm::Sequence *> sequences);
  void mappingsChanged(std::vector<mechanizm::Mapping *> mappings);

protected:
private:
};
} // namespace mechanizm