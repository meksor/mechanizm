#include "source.h"

namespace mechanizm {

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
  id = root["id"].asUInt();
  name = root["name"].asString();
  path = root["path"].asString();
  extension = root["extension"].asString();
  type = static_cast<Type>(root["type"].asInt());
}

} // namespace mechanizm