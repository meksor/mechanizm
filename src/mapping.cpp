#include "mapping.h"

namespace mechanizm {

Json::Value RythmLink::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["rpId"] = rpId;
  root["tsId"] = tsId;
  root["nextInterpolation"] = static_cast<int>(nextInterpolation);
  return root;
}

void RythmLink::SetJsonValue(const Json::Value root) {
  id = root["id"].asUInt();
  rpId = root["rpId"].asUInt();
  tsId = root["tsId"].asUInt();
  nextInterpolation =
      static_cast<Interpolation>(root["nextInterpolation"].asInt());
}

Json::Value Mapping::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["name"] = name;
  root["clipId"] = clipId;
  root["sequenceId"] = sequenceId;

  for (int i = 0; i < rythmLinks.size(); ++i)
    root["rythmLinks"][i] = rythmLinks[i].JsonValue();

  return root;
}

void Mapping::SetJsonValue(const Json::Value root) {
  id = root["id"].asUInt();
  name = root["name"].asString();
  clipId = root["clipId"].asUInt();
  sequenceId = root["sequenceId"].asUInt();

  const Json::Value rythmLinks = root["rythmLinks"];
  for (int i = 0; i < rythmLinks.size(); ++i)
    loadRythmLink(rythmLinks[i]);
  emit rythmLinksChanged(this->rythmLinks);
}

void Mapping::loadRythmLink(Json::Value json) {
  mechanizm::RythmLink rl = mechanizm::RythmLink(json);
  rythmLinks.push_back(rl);
}

} // namespace mechanizm