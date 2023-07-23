#include <QTextStream>
#include <utility>

#include "clip.h"
#include "mapping.h"
#include "project.h"
#include "sequence.h"

namespace mechanizm {

Project::Project(QDir &rootDir) {
  mechanizm::JsonStorable::loadFromDisk(rootDir);
}

Project::Project(QDir &rootDir, std::string name) : name(name) {
  mechanizm::JsonStorable::create(rootDir);
  mechanizm::JsonStorable::saveToDisk();
}

void Project::setDefaults() {
  bpm = 120.;
  timeDivision = std::make_pair(4, 4);
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
  root["name"] = name;
  root["bpm"] = bpm;
  root["timeDivision"][0] = timeDivision.first;
  root["timeDivision"][1] = timeDivision.second;

  for (int i = 0; i < sources.size(); ++i)
    root["sources"][i] = sources[i]->JsonValue();

  for (int i = 0; i < clips.size(); ++i)
    root["clips"][i] = clips[i]->JsonValue();

  for (int i = 0; i < sequences.size(); ++i)
    root["sequences"][i] = sequences[i]->JsonValue();

  for (int i = 0; i < mappings.size(); ++i)
    root["mappings"][i] = mappings[i]->JsonValue();

  return root;
}

void Project::SetJsonValue(const Json::Value root) {
  name = root["name"].asCString();
  bpm = root["bpm"].asDouble();
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
  mechanizm::Source *source = new mechanizm::Source(json);
  sources.push_back(source);
}

void Project::addSource(mechanizm::Source *source) {
  sources.push_back(source);
  emit sourcesChanged(sources);
}

void Project::removeSource(mechanizm::Source *source) {
  auto item = std::find(sources.begin(), sources.end(), source);
  sources.erase(item);
  // TODO: prevent or delete clips/seqs
  emit sourcesChanged(sources);
}

void Project::connectClip(mechanizm::Clip *clip) {
  connect(this, &Project::sourcesChanged, clip,
          &mechanizm::Clip::onSourcesChanged);
  connect(clip, &Clip::updated, [this]() { emit this->clipsChanged(clips); });
};

void Project::loadClip(Json::Value json) {
  mechanizm::Clip *clip = new mechanizm::Clip(json);
  connectClip(clip);
  clips.push_back(clip);
}

void Project::addClip(mechanizm::Clip *clip) {
  clips.push_back(clip);
  connectClip(clip);
  emit clipsChanged(clips);
}

void Project::removeClip(mechanizm::Clip *clip) {
  auto item = std::find(clips.begin(), clips.end(), clip);
  clips.erase(item);
  // TODO: prevent or delete mapping
  emit clipsChanged(clips);
}

void Project::connectSequence(mechanizm::Sequence *seq) {
  connect(this, &Project::sourcesChanged, seq,
          &mechanizm::Sequence::onSourcesChanged);
  connect(seq, &Sequence::updated,
          [this]() { emit this->sequencesChanged(this->sequences); });
};
void Project::loadSequence(Json::Value json) {
  mechanizm::Sequence *sequence = new mechanizm::Sequence(json);
  connectSequence(sequence);
  sequences.push_back(sequence);
}

void Project::addSequence(mechanizm::Sequence *sequence) {
  sequences.push_back(sequence);
  connectSequence(sequence);
  emit sequencesChanged(sequences);
}

void Project::removeSequence(mechanizm::Sequence *seq) {
  auto item = std::find(sequences.begin(), sequences.end(), seq);
  sequences.erase(item);
  // TODO: prevent or delete mapping
  emit sequencesChanged(sequences);
}

void Project::connectMapping(mechanizm::Mapping *m) {
  connect(this, &Project::sequencesChanged, m,
          &mechanizm::Mapping::onSequencesChanged);
  connect(this, &Project::clipsChanged, m,
          &mechanizm::Mapping::onClipsChanged);
  connect(m, &Mapping::updated,
          [this]() { emit this->mappingsChanged(this->mappings); });
}

void Project::loadMapping(Json::Value json) {
  mechanizm::Mapping *mapping = new mechanizm::Mapping(json);
  connectMapping(mapping);
  mappings.push_back(mapping);
}

void Project::addMapping(mechanizm::Mapping *mapping) {
  mappings.push_back(mapping);
  connectMapping(mapping);
  emit mappingsChanged(mappings);
}

void Project::removeMapping(mechanizm::Mapping *mapping) {
  auto item = std::find(mappings.begin(), mappings.end(), mapping);
  mappings.erase(item);
  emit mappingsChanged(mappings);
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