#include "clip.h"
#include <algorithm>

namespace mechanizm {

const mechanizm::id_t RythmicPoint::getNextId(std::vector<RythmicPoint> items) {
  if (items.size() == 0)
    return 0;
  std::vector<mechanizm::id_t> ids(items.size());
  std::transform(items.cbegin(), items.cend(), ids.begin(),
                 [](RythmicPoint i) { return i.id; });
  auto maxId = std::max_element(ids.begin(), ids.end());
  return (*maxId) + 1;
}

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

long Clip::getFirstFrame() {
  if (rythmicPoints.size() == 0)
    return 0;
  std::vector<long> ids(rythmicPoints.size());
  std::transform(rythmicPoints.cbegin(), rythmicPoints.cend(), ids.begin(),
                 [](RythmicPoint i) { return i.frame; });
  auto minFrame = std::min_element(ids.begin(), ids.end());
  return *minFrame;
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

void Clip::addRythmicPoint(mechanizm::RythmicPoint rp) {
  rythmicPoints.push_back(rp);
  auto compare_frame = [rp](mechanizm::RythmicPoint a,
                            mechanizm::RythmicPoint b) {
    return a.frame < b.frame;
  };
  std::sort(rythmicPoints.begin(), rythmicPoints.end(), compare_frame);

  emit updated();
}

void Clip::removeRythmicPoint(mechanizm::RythmicPoint rp) {
  auto id_matches = [rp](mechanizm::RythmicPoint i) { return i.id == rp.id; };
  auto item =
      std::find_if(rythmicPoints.begin(), rythmicPoints.end(), id_matches);
  rythmicPoints.erase(item);
  emit updated();
}

void Clip::onSourcesChanged(std::vector<mechanizm::Source *> sources) {
  auto id_matches = [this](mechanizm::Source *s) {
    return s->id == this->sourceId;
  };
  auto res = std::find_if(sources.begin(), sources.end(), id_matches);
  source = *res;
};

} // namespace mechanizm