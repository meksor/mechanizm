#include "source.h"
#include "json.h"

namespace mechanizm {
const Source::ext_map_t Source::extensionMap = {
    {Source::Type::VIDEO, {".mp4", ".mov", ".mkv"}},
    {Source::Type::MIDI, {".mid", ".midi"}},
};

const Source::dpn_map_t Source::dpNameMap = {
    {Source::Type::VIDEO, "Video"},
    {Source::Type::MIDI, "Midi"},
};

const mechanizm::id_t Source::getNextId(std::vector<Source *> items) {
  if (items.size() == 0)
    return 0;
  std::vector<mechanizm::id_t> ids(items.size());
  std::transform(items.cbegin(), items.cend(), ids.begin(),
                 [](Source *i) { return i->id; });
  auto maxId = std::max_element(ids.begin(), ids.end());
  return (*maxId) + 1;
}

Source::Source(mechanizm::id_t id, std::string path) : path(path) {
  int firstCharIdx = path.find_last_of("/") + 1;
  int lastDotIdx = path.find_last_of(".");
  name = path.substr(firstCharIdx);
  extension = path.substr(lastDotIdx);
  for (auto const &[key, val] : extensionMap) {
    if (val.contains(extension)) {
      type = key;
      break;
    }
  }
}
Json::Value Source::JsonValue() const {
  Json::Value root;
  root["id"] = id;
  root["name"] = name;
  root["path"] = path;
  root["extension"] = extension;
  root["type"] = static_cast<int>(type);
  return root;
}

void Source::SetJsonValue(const Json::Value root) {
  name = root["name"].asString();
  path = root["path"].asString();
  extension = root["extension"].asString();
  type = static_cast<Type>(root["type"].asInt());
}

} // namespace mechanizm