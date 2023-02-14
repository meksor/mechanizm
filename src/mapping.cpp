#include "mapping.h"
#include "json.h"
#include "sequence.h"
#include <cstddef>
#include <json/value.h>
#include <unistd.h>

namespace mechanizm {

Json::Value Mapping::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["name"] = name;
  root["clipId"] = clipId;

  root["channels"] = Json::arrayValue;
  for (int i = 0; i < channels.size(); ++i)
    root["channels"][i] = channels[i].JsonValue();

  return root;
}

void Mapping::SetJsonValue(const Json::Value root) {
  id = root["id"].asLargestUInt();
  name = root["name"].asString();
  clipId = root["clipId"].asLargestUInt();

  const Json::Value channels = root["channels"];
  for (int i = 0; i < channels.size(); ++i)
    this->loadChannel(channels[i]);
}

void Mapping::loadChannel(Json::Value json) {
  mechanizm::Channel c(json);
  channels.push_back(c);
}

void Mapping::onSequencesChanged(std::vector<mechanizm::Sequence *> sequences) {
  std::vector<mechanizm::Channel> staleChannels(this->channels);
  this->channels.clear();
  for (auto const &x : staleChannels) {
    auto id_matches = [this, x](mechanizm::Sequence *s) {
      return s->id == x.sequence->id;
    };

    auto res = std::find_if(sequences.begin(), sequences.end(), id_matches);
    if (res != sequences.end())
      this->channels.push_back(x);
  }
};

void Mapping::onClipsChanged(std::vector<mechanizm::Clip *> clips) {
  auto id_matches = [this](mechanizm::Clip *c) {
    return c->id == this->clipId;
  };
  auto res = std::find_if(clips.begin(), clips.end(), id_matches);
  clip = *res;
};

} // namespace mechanizm