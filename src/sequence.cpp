#include "sequence.h"
#include <cxxmidi/file.hpp>

namespace mechanizm {

static int MIDI_NOTE_ON = 0x90;

const mechanizm::id_t TimeStep::getNextId(std::vector<TimeStep> items) {
  if (items.size() == 0)
    return 0;
  std::vector<mechanizm::id_t> ids(items.size());
  std::transform(items.cbegin(), items.cend(), ids.begin(),
                 [](TimeStep i) { return i.id; });
  auto maxId = std::max_element(ids.begin(), ids.end());
  return (*maxId) + 1;
}

Json::Value TimeStep::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["note"] = note;
  return root;
}

void TimeStep::SetJsonValue(const Json::Value root) {
  id = root["id"].asLargestUInt();
  note = root["note"].asDouble();
}

Json::Value Sequence::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["name"] = name;
  root["sourceId"] = sourceId;

  root["timeSteps"] = Json::arrayValue;
  for (int i = 0; i < timeSteps.size(); ++i)
    root["timeSteps"][i] = timeSteps[i].JsonValue();

  return root;
}

const mechanizm::id_t Sequence::getNextId(std::vector<Sequence *> items) {
  if (items.size() == 0)
    return 0;
  std::vector<mechanizm::id_t> ids(items.size());
  std::transform(items.cbegin(), items.cend(), ids.begin(),
                 [](Sequence *i) { return i->id; });
  auto maxId = std::max_element(ids.begin(), ids.end());
  return (*maxId) + 1;
}

Sequence::Sequence(mechanizm::id_t id, mechanizm::Source *source,
                   cxxmidi::File file, int trackIndex)
    : id(id), source(source), sourceId(source->id) {
  // TODO: static -> configurable in modal

  cxxmidi::Track track = file[trackIndex];
  name = source->name + " " + track.GetName();

  int stepId = 0;
  long currentT = 0;
  for (auto ev : track) {
    currentT += ev.Dt();
    if ((ev[0] & 0xf0) == MIDI_NOTE_ON) {
      TimeStep step(stepId, double(currentT) / file.TimeDivision());
      this->timeSteps.push_back(step);
      stepId++;
    }
  }
};

void Sequence::SetJsonValue(const Json::Value root) {
  id = root["id"].asLargestUInt();
  name = root["name"].asString();
  sourceId = root["sourceId"].asLargestUInt();

  const Json::Value timeSteps = root["timeSteps"];
  for (int i = 0; i < timeSteps.size(); ++i)
    loadTimeStep(timeSteps[i]);
  emit timeStepsChanged(this->timeSteps);
}

void Sequence::loadTimeStep(Json::Value json) {
  mechanizm::TimeStep ts = mechanizm::TimeStep(json);
  timeSteps.push_back(ts);
}

void Sequence::onSourcesChanged(std::vector<mechanizm::Source *> sources) {
  auto id_matches = [this](mechanizm::Source *s) {
    return s->id == this->sourceId;
  };
  auto res = std::find_if(sources.begin(), sources.end(), id_matches);
  source = *res;
};

mechanizm::TimeStep Sequence::getNextTimestep(double pos) {
  for (auto ts : timeSteps) {
    if (ts.note > pos) {
      return ts;
    }
  }
  return mechanizm::TimeStep(-1, -1);
};

} // namespace mechanizm