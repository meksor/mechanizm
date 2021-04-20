#include <QCryptographicHash>
#include <QTextStream>

#include <FFmpegReader.h>
#include <FFmpegWriter.h>

#include "source.h"

namespace mechanizm {

    Source::Source(QFile &importFile, QDir &projectDir) {
        if (importFile.open(QIODevice::ReadOnly)) {
            QByteArray fileData = importFile.readAll();
            fileHash = QCryptographicHash::hash(fileData, QCryptographicHash::Md5).toHex();
            name = fileHash;
        } 

        rootDir = QDir(projectDir.absoluteFilePath("sources/" + fileHash));
        if (!rootDir.exists()) {
            rootDir.mkpath(".");
            rootDir.mkpath("source/");
            rootDir.mkpath("proxy/");
        }

        QString sourceExtension = importFile.fileName().mid(importFile.fileName().lastIndexOf("."));
        filePath = "source/source" + sourceExtension;
        QString absFilePath = rootDir.filePath(filePath);
        
        if (importFile.copy(absFilePath)) {
            this->makeProxyMedia();
            saveToDisk();
        } // TODO: Else Error
    }

    Source::Source(QString dirPath) : rootDir(QDir(dirPath)) {
        QFile jsonFile(rootDir.filePath("source.json"));

        if (jsonFile.open(QIODevice::ReadOnly)) {
            QByteArray fileData = jsonFile.readAll();  
            this->SetJson(fileData.toStdString());
        } else {
            // TODO: Error: Cannot open file.
        }
    }

    void Source::saveToDisk() {
        QFile jsonFile(rootDir.filePath("source.json"));
        if (jsonFile.open(QIODevice::ReadWrite)) {
            QTextStream stream(&jsonFile);
            stream << QString(Json().c_str());
            jsonFile.close();
        }
    }

    QString Source::getRelativePath(QDir dir) {
        return dir.relativeFilePath(rootDir.absolutePath());
    }

    void Source::makeProxyMedia() {
        openshot::FFmpegReader reader(rootDir.filePath(filePath).toStdString());
        reader.Open();

        proxyPath = rootDir.filePath("proxy/480.mp4");

        openshot::FFmpegWriter writer(proxyPath.toStdString());

        int width = reader.info.display_ratio.ToFloat() * 480;
        writer.SetVideoOptions(true, "libx264", reader.info.fps, 
                width, 480, 
                openshot::Fraction(1,1), false, false, -1);

        writer.PrepareStreams();
        writer.SetOption(openshot::VIDEO_STREAM, "tune", "zerolatency" );
        writer.SetOption(openshot::VIDEO_STREAM, "preset", "veryfast" );

        writer.Open();
        writer.WriteFrame(&reader, 1, reader.info.video_length);
        writer.Close();
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
