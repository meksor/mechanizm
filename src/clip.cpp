#include <QFile>
#include <QByteArray>

#include "clip.h"

namespace mechanizm {
    Clip::Clip(QString n, mechanizm::Source::shared_ptr s, QDir &projectDir)
            : name(n), source(s), openshot::Clip() {
        mechanizm::JsonStorable::create(projectDir);
        initReader();
        mechanizm::JsonStorable::saveToDisk();
    }

    Clip::Clip(QDir &rootDir)
            : openshot::Clip() {
        mechanizm::JsonStorable::loadFromDisk(rootDir);
    }

    void Clip::initReader() {
        proxyReader = std::make_shared<openshot::FFmpegReader>(source->getProxyPath().toStdString());
    }

    void Clip::addRythmicPoint(long x, float y) {
        rythmicPoints.AddPoint(x, y);
        mechanizm::JsonStorable::saveToDisk();
    }

    Json::Value Clip::JsonValue() const {
        Json::Value root = openshot::Clip::JsonValue();
        
        root["name"] = name.toStdString();
        root["source"] = source->getRelativePath(rootDir).toStdString();
        root["rythmicPoints"] = rythmicPoints.JsonValue();
        return root;
    }

    void Clip::SetJsonValue(const Json::Value root) {
        openshot::Clip::SetJsonValue(root);

        name = root["name"].asCString();
        QString sourcePath = rootDir.absoluteFilePath(root["source"].asCString());
        QDir sourceDir(sourcePath);
        source = std::make_shared<mechanizm::Source>(sourceDir);
        rythmicPoints.SetJsonValue(root["rythmicPoints"]);
        initReader();
    }
}
