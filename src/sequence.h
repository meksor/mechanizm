#pragma once
#include "json.h"
#include "source.h"
#include <cxxmidi/file.hpp>
#include <string>

namespace mechanizm {
class TimeStep : public mechanizm::JsonSerializable {
public:
  static const mechanizm::id_t getNextId(std::vector<TimeStep>);

  TimeStep(const Json::Value root) { SetJsonValue(root); };
  TimeStep(mechanizm::id_t i, double n) : id(i), note(n){};

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;
  double note;
  mechanizm::id_t id;

protected:
private:
};

class Sequence : public QObject, public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  static const mechanizm::id_t getNextId(std::vector<Sequence *>);

  Sequence(const Json::Value root) { SetJsonValue(root); };
  Sequence(mechanizm::id_t, mechanizm::Source *, cxxmidi::File file,
           int trackIndex);

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  void setName(QString n) {
    name = n.toStdString();
    emit updated();
  };

  mechanizm::TimeStep getNextTimestep(double p);

  void loadTimeStep(Json::Value json);
  void onSourcesChanged(std::vector<mechanizm::Source *>);

  mechanizm::id_t id;
  std::string name;

  mechanizm::id_t sourceId;
  mechanizm::Source *source = nullptr;

  std::vector<mechanizm::TimeStep> timeSteps;

signals:
  void timeStepsChanged(std::vector<mechanizm::TimeStep>);
  void updated();

protected:
private:
};

} // namespace mechanizm