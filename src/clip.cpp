#include <QFile>
#include <QTextStream>
#include <QByteArray>

#include "clip.h"

namespace mechanizm {
    Clip::Clip(QString n, mechanizm::Source::shared_ptr s, QDir &projectDir)
            : name(n), source(s), openshot::Clip() {

        rootDir = QDir(projectDir.absoluteFilePath("clips/" + n));
        if (!rootDir.exists()) {
            rootDir.mkpath(".");
        }

        initReader();
        saveToDisk();
    }

    Clip::Clip(QString dirPath)
            : rootDir(QDir(dirPath)), openshot::Clip() {
        QFile jsonFile(rootDir.filePath("clip.json"));

        if (jsonFile.open(QIODevice::ReadOnly)) {
            QByteArray fileData = jsonFile.readAll();  
            this->SetJson(fileData.toStdString());
            initReader();
        } // TODO: Error: Cannot open file.
    }

    void Clip::initReader() {
        proxyReader = std::make_shared<openshot::FFmpegReader>(source->getProxyPath().toStdString());
    }

    QString Clip::getRelativePath(QDir dir) {
        return dir.relativeFilePath(rootDir.absolutePath());
    }

    void Clip::saveToDisk() {
        QFile jsonFile(rootDir.filePath("clip.json"));
        if (jsonFile.open(QIODevice::ReadWrite)) {
            QTextStream stream(&jsonFile);
            stream << QString(Json().c_str());
            jsonFile.close();
        }
    }

    void Clip::addRythmicPoint(long x, float y) {
        rythmicPoints.AddPoint(x, y);
        saveToDisk();
    }

    Json::Value Clip::JsonValue() const {
        Json::Value root;
        root["name"] = name.toStdString();
        root["source"] = source->getRelativePath(rootDir).toStdString();
        root["rythmicPoints"] = rythmicPoints.JsonValue();
        return root;
    }

    void Clip::SetJsonValue(const Json::Value root) {
        name = root["name"].asCString();
        QString sourcePath = rootDir.absoluteFilePath(root["source"].asCString());
        source = std::make_shared<mechanizm::Source>(sourcePath);
        rythmicPoints.SetJsonValue(root["rythmicPoints"]);
    }
}
