#include <QTextStream>

#include "clip.h"
#include "mapping.h"
#include "project.h"
#include "sequence.h"

namespace mechanizm {

Project::Project(QDir &rootDir) {
  mechanizm::JsonStorable::loadFromDisk(rootDir);
}

Project::Project(QDir &rootDir, QString name) : name(name) {
  mechanizm::JsonStorable::create(rootDir);
  mechanizm::JsonStorable::saveToDisk();
}

void Project::setupDirectory() {
  rootDir.mkpath(".");
  rootDir.mkpath("sources");
  rootDir.mkpath("sequences");
  rootDir.mkpath("clips");
  rootDir.mkpath("mappings");
}

Json::Value Project::JsonValue() const {
  Json::Value root;
  root["name"] = name.toStdString();
  root["bpm"] = bpm;
  root["timeDivision"][0] = timeDivision.first;
  root["timeDivision"][1] = timeDivision.second;

  for (int i = 0; i < sources.size(); ++i)
    root["sources"][i] = sources[i].JsonValue();

  for (int i = 0; i < clips.size(); ++i)
    root["clips"][i] = clips[i].JsonValue();

  for (int i = 0; i < sequences.size(); ++i)
    root["sequences"][i] = sequences[i].JsonValue();

  for (int i = 0; i < mappings.size(); ++i)
    root["mappings"][i] = mappings[i].JsonValue();

  return root;
}

void Project::SetJsonValue(const Json::Value root) {
  name = root["name"].asCString();
  bpm = root["bpm"].asInt();
  timeDivision = std::make_pair<int, int>(root["timeDivision"][0].asInt(),
                                          root["timeDivision"][1].asInt());

  const Json::Value sources = root["sources"];
  for (int i = 0; i < sources.size(); ++i)
    loadSource(sources[i]);
  emit sourcesChanged(this->sources);

  const Json::Value clips = root["clips"];
  for (int i = 0; i < clips.size(); ++i)
    loadClip(clips[i]);
  emit clipsChanged(this->clips);

  const Json::Value sequences = root["sequences"];
  for (int i = 0; i < sequences.size(); ++i)
    loadSequence(sequences[i]);
  emit sequencesChanged(this->sequences);

  const Json::Value mappings = root["mappings"];
  for (int i = 0; i < mappings.size(); ++i)
    loadMapping(mappings[i]);
  emit mappingsChanged(this->mappings);
}

void Project::loadSource(Json::Value json) {
  mechanizm::Source source = mechanizm::Source(json);
  sources.push_back(source);
}
void Project::loadClip(Json::Value json) {
  mechanizm::Clip clip = mechanizm::Clip(json);
  clips.push_back(clip);
}

void Project::loadSequence(Json::Value json) {
  mechanizm::Sequence sequence = mechanizm::Sequence(json);
  sequences.push_back(sequence);
}

void Project::loadMapping(Json::Value json) {
  mechanizm::Mapping mapping = mechanizm::Mapping(json);
  mappings.push_back(mapping);
}

} // namespace mechanizm

/*

void Project::importSourceFile(QString filePath) {
  QFile impFile(filePath);
  auto source = std::make_shared<mechanizm::Source>(impFile, rootDir);
  sources.push_back(source);
  emit sourcesChanged(sources);
  mechanizm::JsonStorable::saveToDisk();
}

void Project::newClip(mechanizm::Source::shared_ptr source) {
  auto clip = std::make_shared<mechanizm::Clip>(QString::number(clips.size()),
                                                source, rootDir);
  clips.push_back(clip);
  emit clipsChanged(clips);
  mechanizm::JsonStorable::saveToDisk();
}

void Project::newMapping(mechanizm::Clip::shared_ptr clip,
                         mechanizm::Sequence::shared_ptr sequence) {
  auto mapping = std::make_shared<mechanizm::Mapping>(clip, sequence,
                                                      clip->getName(), rootDir);
  mappings.push_back(mapping);
  emit mappingsChanged(mappings);
  mechanizm::JsonStorable::saveToDisk();
}

*/