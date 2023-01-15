#pragma once
#include "ReaderBase.h"
#include "json.h"
#include "source.h"
#include <QObject>
#include <memory>
#include <string>

namespace mechanizm {
class RythmicPoint : public mechanizm::JsonSerializable {
public:
  RythmicPoint(const Json::Value root) { SetJsonValue(root); };

  double getSeconds(openshot::ReaderInfo info) {
    return frame / info.fps.ToDouble();
  }

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  mechanizm::id_t id;
  int frame;

protected:
private:
};

class Clip : public QObject, public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  static const mechanizm::id_t getNextId(std::vector<Clip *> items);

  Clip(const Json::Value root) { SetJsonValue(root); };
  Clip(mechanizm::id_t, mechanizm::Source *);
  virtual ~Clip(){};

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  void setName(QString n) {
    name = n.toStdString();
    emit updated();
  };

  // std::vector<mechanizm::RythmicPoint> eclipsedRythmicPoints();
  void loadRythmicPoint(Json::Value json);
  void onSourcesChanged(std::vector<mechanizm::Source *>);

  mechanizm::id_t id;
  std::string name;

  mechanizm::id_t sourceId;
  mechanizm::Source *source = nullptr;

  int inFrame;
  int outFrame;

  std::vector<mechanizm::RythmicPoint> rythmicPoints;
signals:
  void updated();
  void rythmicPointsChanged(std::vector<mechanizm::RythmicPoint> rythmicPoints);

protected:
private:
};

} // namespace mechanizm