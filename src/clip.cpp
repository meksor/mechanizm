#include "clip.h"

namespace mechanizm {

Json::Value RythmicPoint::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["frame"] = frame;
  return root;
}

void RythmicPoint::SetJsonValue(const Json::Value root) {
  id = root["id"].asUInt();
  frame = root["frame"].asUInt();
}

Json::Value Clip::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["name"] = name;
  root["sourceId"] = sourceId;
  root["inFrame"] = inFrame;
  root["outFrame"] = outFrame;

  for (int i = 0; i < rythmicPoints.size(); ++i)
    root["rythmicPoints"][i] = rythmicPoints[i].JsonValue();

  return root;
}

void Clip::SetJsonValue(const Json::Value root) {
  id = root["id"].asUInt();
  name = root["name"].asString();
  sourceId = root["sourceId"].asUInt();
  inFrame = root["inFrame"].asUInt();
  outFrame = root["outFrame"].asUInt();

  const Json::Value rythmicPoints = root["rythmicPoints"];
  for (int i = 0; i < rythmicPoints.size(); ++i)
    loadRythmicPoint(rythmicPoints[i]);
  emit rythmicPointsChanged(this->rythmicPoints);
}

void Clip::loadRythmicPoint(Json::Value json) {
  mechanizm::RythmicPoint rp = mechanizm::RythmicPoint(json);
  rythmicPoints.push_back(rp);
}

} // namespace mechanizm