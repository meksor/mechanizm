#include "mapping.h"
#include "json.h"
#include "sequence.h"
#include <cstddef>
#include <json/value.h>
#include <limits>
#include <tuple>

namespace mechanizm {

const mechanizm::id_t Channel::getNextId(std::vector<Channel> items) {
  if (items.size() == 0)
    return 0;
  std::vector<mechanizm::id_t> ids(items.size());
  std::transform(items.cbegin(), items.cend(), ids.begin(),
                 [](Channel i) { return i.id; });
  auto maxId = std::max_element(ids.begin(), ids.end());
  return (*maxId) + 1;
}

const mechanizm::id_t Mapping::getNextId(std::vector<Mapping *> items) {
  if (items.size() == 0)
    return 0;
  std::vector<mechanizm::id_t> ids(items.size());
  std::transform(items.cbegin(), items.cend(), ids.begin(),
                 [](Mapping *i) { return i->id; });
  auto maxId = std::max_element(ids.begin(), ids.end());
  return (*maxId) + 1;
}

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

void Mapping::addChannel(mechanizm::Channel channel) {
  channels.push_back(channel);
}

void Mapping::removeChannel(mechanizm::Channel channel) {
  auto item = std::find(channels.begin(), channels.end(), channel);
  channels.erase(item);
}
void Mapping::onChannelsChanged() {}

std::pair<mechanizm::TimeStep, const mechanizm::Channel *>
getNextTimestep(std::vector<mechanizm::Channel> channels, double pos) {

  double minDt = std::numeric_limits<double>::max();
  mechanizm::TimeStep minTs(-1, 0);
  const mechanizm::Channel *minChannel = nullptr;
  for (auto const &c : channels) {
    mechanizm::TimeStep ts = c.sequence->getNextTimestep(pos);
    double dt = ts.note - pos;
    if (dt < minDt) {
      minDt = dt;
      minTs = ts;
      minChannel = &c;
    }
  }
  return {minTs, minChannel};
}
Mapping::channelts_t Mapping::getChannelTimeseries() {
  channelts_t tseries;
  double pos = 0;
  mechanizm::TimeStep tstep(-1, -1);
  do {
    auto next = getNextTimestep(channels, pos);
    tstep = next.first;
    tseries.push_back(next);
  } while (tstep.id != -1);

  return tseries;
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