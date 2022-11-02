#pragma once
#include "clip.h"
#include "json.h"
#include "sequence.h"
#include "source.h"
#include <string>

namespace mechanizm {

class RythmLink : public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  enum Interpolation : int { DEFAULT = 0 };

  RythmLink(const Json::Value root) { SetJsonValue(root); };

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

protected:
private:
  mechanizm::id_t id;
  mechanizm::id_t rpId;
  mechanizm::id_t tsId;
  Interpolation nextInterpolation;

  double note;
};

class Mapping : public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  Mapping(const Json::Value root) { SetJsonValue(root); };

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  void loadRythmLink(Json::Value json);

signals:
  void rythmLinksChanged(std::vector<mechanizm::RythmLink> rythmLinks);

protected:
private:
  mechanizm::id_t id;
  std::string name;

  mechanizm::id_t clipId;
  mechanizm::id_t sequenceId;

  mechanizm::Clip *clip = nullptr;
  mechanizm::Sequence *sequence = nullptr;

  std::vector<mechanizm::RythmLink> rythmLinks;
};

} // namespace mechanizm