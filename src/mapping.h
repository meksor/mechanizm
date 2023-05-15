#pragma once
#include "clip.h"
#include "json.h"
#include "sequence.h"
#include "source.h"
#include <string>

namespace mechanizm {

class Channel : public mechanizm::JsonSerializable {
public:
  enum Effect : int { INC = 0, DEC = 1, SEEK = 2, SET = 3 };

  Channel(const Json::Value root) { SetJsonValue(root); };
  Channel(mechanizm::id_t sid, Effect e, int v)
      : sequenceId(sid), effect(e), value(v){};

  Json::Value JsonValue() const override {
    Json::Value root;
    root["sequenceId"] = sequenceId;
    root["effect"] = effect;
    root["value"] = value;
    return root;
  };

  void SetJsonValue(const Json::Value root) override {
    sequenceId = root["sequenceId"].asLargestUInt();
    effect = (Effect)root["effect"].asLargestUInt();
    value = root["value"].asLargestInt();
  };

  void onSequencesChanged(std::vector<mechanizm::Sequence *>);

  mechanizm::Sequence *sequence;
  mechanizm::id_t sequenceId;
  Effect effect;
  int value;

protected:
private:
};

class Mapping : public QObject, public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  static const mechanizm::id_t getNextId(std::vector<Mapping *>);

  Mapping(const Json::Value root) { SetJsonValue(root); };
  Mapping(mechanizm::id_t i, std::string n, mechanizm::Clip *c) 
    : id(i), name(n), clip(c), clipId(c->id) {};

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;
  void loadChannel(Json::Value json);

  void onSequencesChanged(std::vector<mechanizm::Sequence *>);
  void onClipsChanged(std::vector<mechanizm::Clip *>);

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