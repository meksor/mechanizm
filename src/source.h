#pragma once
#include "json.h"
#include <string>

namespace mechanizm {

class Source : public mechanizm::JsonSerializable {
  Q_OBJECT
public:
  enum Type : int { VIDEO = 0, MIDI = 1 };
  Source(const Json::Value root) { SetJsonValue(root); };

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

protected:
private:
  mechanizm::id_t id;
  std::string name;
  std::string extension; // string after last "." of path
  std::string path;      // original file path
  Type type;

  // TODO: proxy media etc.
};

} // namespace mechanizm