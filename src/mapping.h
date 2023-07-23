#pragma once
#include "clip.h"
#include "json.h"
#include "sequence.h"
#include "source.h"
#include <libopenshot/Clip.h>
#include <string>
#include <vector>

namespace mechanizm {

class Channel : public mechanizm::JsonSerializable {
public:
  enum Effect : int { INC = 0, DEC = 1, SET = 2 };
  enum Interpolation : int { NONE = 0, LINEAR = 1 };
  const mechanizm::id_t getNextId(std::vector<Channel> items);

  Channel(const Json::Value root) { SetJsonValue(root); };
  Channel(mechanizm::Sequence *s, Effect e = Effect::INC,
          Interpolation i = Interpolation::LINEAR, int v = 0)
      : sequence(s), sequenceId(s->id), effect(e), interpolation(i), value(v) {
    name = sequence->name;
  };
  bool operator==(const Channel &o) const { return o.id == this->id; }

  Json::Value JsonValue() const override {
    Json::Value root;
    root["sequenceId"] = sequenceId;
    root["effect"] = effect;
    root["interpolation"] = interpolation;
    root["value"] = value;
    root["name"] = name;
    return root;
  };

  void SetJsonValue(const Json::Value root) override {
    sequenceId = root["sequenceId"].asLargestUInt();
    effect = (Effect)root["effect"].asLargestUInt();
    interpolation = (Interpolation)root["interpolation"].asLargestUInt();
    value = root["value"].asLargestInt();
    name = root["name"].asString();
  };

  mechanizm::Sequence *sequence;
  mechanizm::id_t sequenceId;
  Effect effect;
  Interpolation interpolation;
  int value;
  std::string name;
  mechanizm::id_t id;

protected:
private:
};

class Mapping : public QObject, public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  static const mechanizm::id_t getNextId(std::vector<Mapping *>);
  typedef std::vector<
      std::tuple<mechanizm::TimeStep, const mechanizm::Channel *>>
      channelts_t;

  Mapping(const Json::Value root) { SetJsonValue(root); };
  Mapping(mechanizm::id_t i, std::string n, mechanizm::Clip *c)
      : id(i), name(n), clip(c), clipId(c->id){};

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;
  void loadChannel(Json::Value json);
  void addChannel(mechanizm::Channel c);
  void removeChannel(mechanizm::Channel c);

  void onSequencesChanged(std::vector<mechanizm::Sequence *>);
  void onClipsChanged(std::vector<mechanizm::Clip *>);
  void onChannelsChanged();

  channelts_t getChannelTimeseries();

  void setName(QString n) {
    name = n.toStdString();
    emit updated();
  };

  mechanizm::id_t id;
  std::string name;

  mechanizm::id_t clipId;
  mechanizm::Clip *clip = nullptr;
  std::vector<mechanizm::Channel> channels;

signals:
  void updated();

protected:
private:
};

} // namespace mechanizm