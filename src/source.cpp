#include <algorithm>
#include <iostream>
#include <QCryptographicHash>
#include <QTranslator>

#include <FFmpegReader.h>
#include <FFmpegWriter.h>

#include "source.h"

namespace mechanizm {

    Source::Source(QFile &importFile, QDir &projectDir) {
        if (importFile.open(QIODevice::ReadOnly)) {
            QByteArray fileData = importFile.readAll();
            fileHash = QCryptographicHash::hash(fileData, QCryptographicHash::Md5).toHex();
        } 
        name = importFile.fileName().mid(importFile.fileName().lastIndexOf("/"));

        mechanizm::JsonStorable::create(projectDir);

        QString sourceExtension = importFile.fileName().mid(importFile.fileName().lastIndexOf("."));
        filePath = "source/source" + sourceExtension;
        QString absFilePath = rootDir.filePath(filePath);
        
        if (importFile.copy(absFilePath)) {
            this->makeProxyMedia();
            mechanizm::JsonStorable::saveToDisk();
        } // TODO: Else Error
    }

    Source::Source(QDir &rootDir) {
        mechanizm::JsonStorable::loadFromDisk(rootDir);
    }

    void Source::makeProxyMedia() {
        openshot::FFmpegReader reader(rootDir.filePath(filePath).toStdString());
        reader.Open();
        int height = std::min(reader.info.height, 480);

        proxyPath = QString( "./proxy/%1.mp4").arg(height);

        openshot::FFmpegWriter writer(rootDir.filePath(proxyPath).toStdString());

        int width = reader.info.display_ratio.ToFloat() * height;
        writer.SetVideoOptions(true, "libx264", reader.info.fps, 
                width, height, 
                openshot::Fraction(1,1), false, false, -1);

        writer.PrepareStreams();
        writer.SetOption(openshot::VIDEO_STREAM, "tune", "zerolatency" );
        writer.SetOption(openshot::VIDEO_STREAM, "preset", "veryfast" );

        writer.Open();
        writer.WriteFrame(&reader, 1, reader.info.video_length);
        writer.Close();
    }

    void Source::setupDirectory() {
        rootDir.mkpath(".");
        rootDir.mkpath("source/");
        rootDir.mkpath("proxy/");
    }

    Json::Value Source::JsonValue() const {
        Json::Value root;
        root["name"] = name.toStdString();
        root["filePath"] = filePath.toStdString();
        root["fileHash"] = fileHash.toStdString();
        root["proxyPath"] = proxyPath.toStdString();

        return root;
    }

    void Source::SetJsonValue(const Json::Value root) {
        name = root["name"].asCString();
        filePath = root["filePath"].asCString();
        fileHash = root["fileHash"].asCString();
        proxyPath = root["proxyPath"].asCString();
    }

}
