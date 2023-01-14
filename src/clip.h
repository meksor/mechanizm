#pragma once
#include "json.h"
#include "source.h"
#include <QObject>
#include <memory>
#include <string>

namespace mechanizm {
class RythmicPoint : public mechanizm::JsonSerializable {
public:
  RythmicPoint(const Json::Value root) { SetJsonValue(root); };

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

protected:
private:
  mechanizm::id_t id;
  int frame;
};

class Clip : public QObject, public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  Clip(const Json::Value root) { SetJsonValue(root); };
  virtual ~Clip(){};

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  // std::vector<mechanizm::RythmicPoint> eclipsedRythmicPoints();
  void loadRythmicPoint(Json::Value json);

signals:
  void rythmicPointsChanged(std::vector<mechanizm::RythmicPoint> rythmicPoints);

protected:
private:
  mechanizm::id_t id;
  std::string name;

  mechanizm::id_t sourceId;
  // pointer to source in Project.sources TODO
  mechanizm::Source *source = nullptr;

  int inFrame;
  int outFrame;

  std::vector<mechanizm::RythmicPoint> rythmicPoints;
};

} // namespace mechanizm