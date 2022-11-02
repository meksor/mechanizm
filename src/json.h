#pragma once
#include <Json.h>
#include <QDir>
#include <QString>

namespace mechanizm {
typedef unsigned long id_t;

class JsonSerializable {
public:
  std::string Json() const;
  void SetJson(const std::string value);

  virtual Json::Value JsonValue() const = 0;
  virtual void SetJsonValue(const Json::Value root) = 0;
};

class JsonStorable : public JsonSerializable {
public:
  QString getRelativePath(QDir dir);

protected:
  void create(QDir &projectDir);
  void loadFromDisk(QDir &rootDir);
  void saveToDisk();

  virtual QString getDirectoryPath() const = 0;
  virtual QString getJsonFileName() const = 0;
  virtual void setupDirectory();

  QDir rootDir;
};

} // namespace mechanizm