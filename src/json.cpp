
#include <Exceptions.h>
#include <QFile>
#include <QTextStream>

#include "json.h"

namespace mechanizm {
std::string JsonSerializable::Json() const {
  return this->JsonValue().toStyledString();
}

void JsonSerializable::SetJson(const std::string value) {
  try {
    const Json::Value root = openshot::stringToJson(value);
    this->SetJsonValue(root);
  } catch (const std::exception &e) {
    throw openshot::InvalidJSON(
        "JSON is invalid (missing keys or invalid data types)");
  }
};

void JsonStorable::create(QDir &projectDir) {
  rootDir = QDir(projectDir.absoluteFilePath(this->getDirectoryPath()));
  if (!rootDir.exists()) {
    this->setupDirectory();
  }
}

void JsonStorable::loadFromDisk(QDir &rd) {
  rootDir = rd;
  QFile jsonFile(rootDir.filePath(this->getJsonFileName()));
  if (jsonFile.open(QIODevice::ReadOnly)) {
    QByteArray fileData = jsonFile.readAll();
    this->SetJson(fileData.toStdString());
  } // TODO: Error: Cannot open file.
}

void JsonStorable::saveToDisk() {
  QFile jsonFile(rootDir.filePath(this->getJsonFileName()));
  if (jsonFile.open(QIODevice::ReadWrite)) {
    QTextStream stream(&jsonFile);
    stream << QString(this->Json().c_str());
    jsonFile.close();
  }
}

void JsonStorable::setupDirectory() { rootDir.mkpath("."); }

QString JsonStorable::getRelativePath(QDir dir) {
  return dir.relativeFilePath(rootDir.absolutePath());
}
} // namespace mechanizm