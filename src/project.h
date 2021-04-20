#pragma once
#include <QDir>
#include <QObject>
#include <QString>
#include <Json.h>
#include <vector>
#include <memory>
#include "clip.h"
#include "json.h"
#include "source.h"
#include "sequence.h"

namespace mechanizm {
    class Project : public QObject, public mechanizm::JsonSerializable {
        Q_OBJECT
    public:
        typedef std::shared_ptr<Project> shared_ptr;

        Project(QString dirPath);
        Project(QString dirPath, QString name);

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;

        void saveToDisk();
        void loadFromDisk();
        void importSourceFile(QString filePath) ;
        void importSequenceFile(QString filePath) ;
        void newClip(mechanizm::Source::shared_ptr source);        
        void addSource(QString dirPath);
        void addSequence(Json::Value sequenceJson);
        void addClip(QString dirPath);
    
    signals:
        void sourcesChanged(std::vector<mechanizm::Source::shared_ptr> sources);
        void sequencesChanged(std::vector<mechanizm::Sequence::shared_ptr> sequences);
        void clipsChanged(std::vector<mechanizm::Clip::shared_ptr> clips);

    private: 
        std::vector<mechanizm::Source::shared_ptr> sources;
        std::vector<mechanizm::Sequence::shared_ptr> sequences;
        std::vector<mechanizm::Clip::shared_ptr> clips;

        QDir rootDir;
        QString name;
    };
}
