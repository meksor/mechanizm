#pragma once
#include "json.h"
#include "source.h"
#include <string>

namespace mechanizm {
class TimeStep : public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  TimeStep(const Json::Value root) { SetJsonValue(root); };

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

protected:
private:
  mechanizm::id_t id;
  double note;
};

class Sequence : public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  Sequence(const Json::Value root) { SetJsonValue(root); };

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  void loadTimeStep(Json::Value json);

signals:
  void timeStepsChanged(std::vector<mechanizm::TimeStep> timeSteps);

protected:
private:
  mechanizm::id_t id;
  std::string name;

  mechanizm::id_t sourceId;
  // pointer to source in Project.sources TODO
  mechanizm::Source *source = nullptr;

  std::vector<mechanizm::TimeStep> timeSteps;
};

} // namespace mechanizm