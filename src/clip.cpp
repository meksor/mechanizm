#include "clip.h"
#include <algorithm>

namespace mechanizm {

Json::Value RythmicPoint::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["frame"] = frame;
  return root;
}

void RythmicPoint::SetJsonValue(const Json::Value root) {
  id = root["id"].asLargestUInt();
  frame = root["frame"].asLargestUInt();
}

const mechanizm::id_t Clip::getNextId(std::vector<Clip *> items) {
  if (items.size() == 0)
    return 0;
  std::vector<mechanizm::id_t> ids(items.size());
  std::transform(items.cbegin(), items.cend(), ids.begin(),
                 [](Clip *i) { return i->id; });
  auto maxId = std::max_element(ids.begin(), ids.end());
  return (*maxId) + 1;
}

Clip::Clip(mechanizm::id_t i, mechanizm::Source *s) : id(i), source(s) {
  name = source->name;
  sourceId = source->id;
  inFrame = 0;
  outFrame = source->reader->info.video_length;
};

Json::Value Clip::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["name"] = name;
  root["sourceId"] = sourceId;
  root["inFrame"] = inFrame;
  root["outFrame"] = outFrame;

  root["rythmicPoints"] = Json::arrayValue;
  for (int i = 0; i < rythmicPoints.size(); ++i)
    root["rythmicPoints"][i] = rythmicPoints[i].JsonValue();

  return root;
}

void Clip::SetJsonValue(const Json::Value root) {
  id = root["id"].asLargestUInt();
  name = root["name"].asString();
  sourceId = root["sourceId"].asLargestUInt();
  inFrame = root["inFrame"].asLargestUInt();
  outFrame = root["outFrame"].asLargestUInt();

  const Json::Value rythmicPoints = root["rythmicPoints"];
  for (int i = 0; i < rythmicPoints.size(); ++i)
    loadRythmicPoint(rythmicPoints[i]);
  emit rythmicPointsChanged(this->rythmicPoints);
}

void Clip::loadRythmicPoint(Json::Value json) {
  mechanizm::RythmicPoint rp = mechanizm::RythmicPoint(json);
  rythmicPoints.push_back(rp);
}

void Clip::onSourcesChanged(std::vector<mechanizm::Source *> sources) {
  auto id_matches = [this](mechanizm::Source *s) {
    return s->id == this->sourceId;
  };
  auto res = std::find_if(sources.begin(), sources.end(), id_matches);
  source = *res;
};

} // namespace mechanizm