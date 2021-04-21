#include <QTextStream>

#include "project.h"

namespace mechanizm {

    Project::Project(QDir &rootDir) {
        mechanizm::JsonStorable::loadFromDisk(rootDir);
    }

    Project::Project(QDir &rootDir, QString name) : name(name) {
        mechanizm::JsonStorable::create(rootDir);
        mechanizm::JsonStorable::saveToDisk();
    }


     void Project::importSourceFile(QString filePath) {
        QFile impFile(filePath);
        auto source = std::make_shared<mechanizm::Source>(impFile, rootDir);
        sources.push_back(source);
        emit sourcesChanged(sources);
        mechanizm::JsonStorable::saveToDisk();
    }

    void Project::importSequenceFile(QString filePath) {
        QFile impFile(filePath);
        auto sequence = std::make_shared<mechanizm::Sequence>(impFile, 0, rootDir); // TODO: Track Selector
        sequences.push_back(sequence);
        emit sequencesChanged(sequences);
        mechanizm::JsonStorable::saveToDisk();
    }

    void Project::newClip(mechanizm::Source::shared_ptr source) {
        auto clip = std::make_shared<mechanizm::Clip>(QString::number(clips.size()), source, rootDir);
        clips.push_back(clip);
        emit clipsChanged(clips);
        mechanizm::JsonStorable::saveToDisk();
    }

    void Project::newMapping(mechanizm::Clip::shared_ptr clip, mechanizm::Sequence::shared_ptr sequence) {
        auto mapping = std::make_shared<mechanizm::Mapping>(clip, sequence, clip->getName() + " -> " + sequence->getName(), rootDir);
        mappings.push_back(mapping);
        emit mappingsChanged(mappings);
        mechanizm::JsonStorable::saveToDisk();
    }

    void Project::loadSource(QString dirPath) {
        QDir sourceDir(rootDir.filePath(dirPath));
        if (sourceDir.exists()) {
            auto source = std::make_shared<mechanizm::Source>(sourceDir);
            sources.push_back(source);
        } // TODO: else error
    }

    void Project::loadSequence(QString dirPath) {
        QDir sequenceDir(rootDir.filePath(dirPath));
        if (sequenceDir.exists()) {
            auto sequence = std::make_shared<mechanizm::Sequence>(sequenceDir);
            sequences.push_back(sequence);
        } // TODO: else error
    }

    void Project::loadClip(QString dirPath) {
        QDir clipDir(rootDir.filePath(dirPath));
        if (clipDir.exists()) {
            auto clip = std::make_shared<mechanizm::Clip>(clipDir);
            clips.push_back(clip);
        } // TODO: else error
    }

    void Project::loadMapping(QString dirPath) {
        QDir mappingDir(rootDir.filePath(dirPath));
        if (mappingDir.exists()) {
            auto mapping = std::make_shared<mechanizm::Mapping>(mappingDir);
            mappings.push_back(mapping);
        } // TODO: else error
    }

    void Project::setupDirectory() {
        rootDir.mkpath(".");
        rootDir.mkpath("sources");
        rootDir.mkpath("sequences");
        rootDir.mkpath("clips");
        rootDir.mkpath("mappings");
    }

    Json::Value Project::JsonValue() const {
        Json::Value root;
        root["name"] = name.toStdString();

        for (int i = 0; i < sources.size(); ++i )
            root["sources"][i] = sources[i]->getRelativePath(rootDir).toStdString();

        for (int i = 0; i < sequences.size(); ++i )
            root["sequences"][i] = sequences[i]->getRelativePath(rootDir).toStdString();

        for (int i = 0; i < clips.size(); ++i )
            root["clips"][i] = clips[i]->getRelativePath(rootDir).toStdString();
        
        for (int i = 0; i < mappings.size(); ++i )
            root["mappings"][i] = mappings[i]->getRelativePath(rootDir).toStdString();

        return root;
    }

    void Project::SetJsonValue(const Json::Value root) {
        name = root["name"].asCString();

        const Json::Value sourcePaths = root["sources"];
        for (int i = 0; i < sourcePaths.size(); ++i )
            loadSource(QString(sourcePaths[i].asCString()));
        emit sourcesChanged(sources);

        const Json::Value sequencesPaths = root["sequences"];
        for (int i = 0; i < sequencesPaths.size(); ++i )
            loadSequence(sequencesPaths[i].asCString());
        emit sequencesChanged(sequences);

        const Json::Value clipsPaths = root["clips"];
        for (int i = 0; i < clipsPaths.size(); ++i )
            loadClip(QString(clipsPaths[i].asCString()));
        emit clipsChanged(clips);

        const Json::Value mappingsPaths = root["clips"];
        for (int i = 0; i < mappingsPaths.size(); ++i )
            loadMapping(QString(mappingsPaths[i].asCString()));
        emit mappingsChanged(mappings);

    }
       
    void Project::emitAllSignals() {
        emit sourcesChanged(sources);
        emit sequencesChanged(sequences);
        emit clipsChanged(clips);
        emit mappingsChanged(mappings);
    }
}
