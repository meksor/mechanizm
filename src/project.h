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
  Project(QDir &rootDir, QString name);
  virtual ~Project();

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  virtual QString getDirectoryPath() { return "."; };
  virtual QString getJsonFileName() { return "project.json"; };
  virtual void setupDirectory() override;

  void loadSource(Json::Value json);
  void loadClip(Json::Value json);
  void loadSequence(Json::Value json);
  void loadMapping(Json::Value json);

signals:
  void sourcesChanged(std::vector<mechanizm::Source> sources);
  void clipsChanged(std::vector<mechanizm::Clip> clips);
  void sequencesChanged(std::vector<mechanizm::Sequence> sequences);
  void mappingsChanged(std::vector<mechanizm::Mapping> mappings);

protected:
private:
  std::vector<mechanizm::Source> sources;
  std::vector<mechanizm::Clip> clips;
  std::vector<mechanizm::Sequence> sequences;
  std::vector<mechanizm::Mapping> mappings;

  QString name;
  int bpm;
  std::pair<int, int> timeDivision;
};
} // namespace mechanizm