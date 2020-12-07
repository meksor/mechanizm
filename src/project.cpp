#include <QTextStream>

#include "project.h"

namespace mechanizm {

    Project::Project(QString dirPath) : rootDir(QDir(dirPath)) {
    }

    Project::Project(QString dirPath, QString name) : rootDir(QDir(dirPath)), name(name) {
        if (!rootDir.exists()) {
            rootDir.mkpath(".");
            rootDir.mkpath("sources");
            rootDir.mkpath("clips");
        }
        saveToDisk();
    }

    void Project::saveToDisk() {
        QFile jsonFile(rootDir.filePath("project.json"));
        if (jsonFile.open(QIODevice::ReadWrite)) {
            QTextStream stream(&jsonFile);
            stream << QString(Json().c_str());
            jsonFile.close();
        }
    }

    void Project::loadFromDisk() {
        QFile jsonFile(rootDir.filePath("project.json"));
        if (jsonFile.open(QIODevice::ReadOnly)) {
            this->SetJson(jsonFile.readAll().toStdString());
        }
    }

     void Project::importFile(QString filePath) {
        QFile impFile(filePath);
        auto source = std::make_shared<mechanizm::Source>(impFile, rootDir);
        sources.push_back(source);
        emit sourcesChanged(sources);
        saveToDisk();
    }

    void Project::newClip(mechanizm::Source::shared_ptr source) {
        auto clip = std::make_shared<mechanizm::Clip>(QString::number(clips.size()), source, rootDir);
        clips.push_back(clip);
        emit clipsChanged(clips);
        saveToDisk();
    }

    Json::Value Project::JsonValue() const {
        Json::Value root;
        root["name"] = name.toStdString();

        for (int i = 0; i < sources.size(); ++i )
            root["sources"][i] = sources[i]->getRelativePath(rootDir).toStdString();
        
        for (int i = 0; i < clips.size(); ++i )
            root["clips"][i] = clips[i]->getRelativePath(rootDir).toStdString();
            
        return root;
    }

    void Project::SetJsonValue(const Json::Value root) {
        name = root["name"].asCString();

        const Json::Value sourcePaths = root["sources"];
        for (int i = 0; i < sourcePaths.size(); ++i )
            addSource(QString(sourcePaths[i].asCString()));
        emit sourcesChanged(sources);

        const Json::Value clipsPaths = root["clips"];
        for (int i = 0; i < clipsPaths.size(); ++i )
            addClip(QString(clipsPaths[i].asCString()));
        emit clipsChanged(clips);

    }
       
    void Project::addSource(QString dirPath) {
        QDir sourceDir(rootDir.filePath(dirPath));
        if (sourceDir.exists()) {
            auto source = std::make_shared<mechanizm::Source>(sourceDir.absolutePath());
            sources.push_back(source);
        } // TODO: else error
    }

    void Project::addClip(QString dirPath) {
        QDir clipDir(rootDir.filePath(dirPath));
        if (clipDir.exists()) {
            auto clip = std::make_shared<mechanizm::Clip>(clipDir.absolutePath());
            clips.push_back(clip);
        } // TODO: else error
    }

}

