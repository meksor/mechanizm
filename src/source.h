#pragma once
#include "json.h"
#include <FFmpegReader.h>
#include <QObject>
#include <QString>
#include <map>
#include <set>
#include <string>

namespace mechanizm {
class Source : public mechanizm::JsonSerializable, mechanizm::HasId {
public:
  enum Type : int { VIDEO = 0, MIDI = 1 };

  typedef std::map<Source::Type, std::set<std::string>> ext_map_t;
  typedef std::map<Source::Type, std::string> dpn_map_t;
  static const Source::ext_map_t &getExtentionMap();
  static const Source::dpn_map_t &getdpNameMap();

  static const mechanizm::id_t getNextId(std::vector<Source *> items);

  Source(const Json::Value root) { SetJsonValue(root); };
  Source(mechanizm::id_t id, std::string path);

  Json::Value JsonValue() const override;
  void SetJsonValue(const Json::Value root) override;

  void initReader();

  mechanizm::id_t id;
  std::string name;
  std::string extension; // string with last "." of path
  std::string path;      // original file path
  Type type;

  openshot::FFmpegReader *reader;

protected:
private:
  // TODO: proxy media etc.
};

} // namespace mechanizm