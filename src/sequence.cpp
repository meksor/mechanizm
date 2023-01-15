#include "sequence.h"

namespace mechanizm {

Json::Value TimeStep::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["frame"] = note;
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

  for (int i = 0; i < timeSteps.size(); ++i)
    root["timeSteps"][i] = timeSteps[i].JsonValue();

  return root;
}

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

} // namespace mechanizm