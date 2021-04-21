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
#include "mapping.h"

namespace mechanizm {
    class Project : public QObject, public mechanizm::JsonStorable {
        Q_OBJECT
    public:
        typedef std::shared_ptr<Project> shared_ptr;

        Project(QDir &rootDir);
        Project(QDir &rootDir, QString name);

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;

        void importSourceFile(QString filePath) ;
        void importSequenceFile(QString filePath) ;
        void newClip(mechanizm::Source::shared_ptr source);   
        void newMapping(mechanizm::Clip::shared_ptr clip, mechanizm::Sequence::shared_ptr sequence);     
        void loadSource(QString dirPath);
        void loadSequence(QString dirPath);
        void loadClip(QString dirPath);
        void loadMapping(QString dirPath);
        void emitAllSignals();
    
    signals:
        void sourcesChanged(std::vector<mechanizm::Source::shared_ptr> sources);
        void sequencesChanged(std::vector<mechanizm::Sequence::shared_ptr> sequences);
        void clipsChanged(std::vector<mechanizm::Clip::shared_ptr> clips);
        void mappingsChanged(std::vector<mechanizm::Mapping::shared_ptr> clips);

    protected:
        virtual QString getDirectoryPath() const override { return "."; };
        virtual QString getJsonFileName() const override { return "project.json"; };
        virtual void setupDirectory() override;

    private: 
        std::vector<mechanizm::Source::shared_ptr> sources;
        std::vector<mechanizm::Sequence::shared_ptr> sequences;
        std::vector<mechanizm::Clip::shared_ptr> clips;
        std::vector<mechanizm::Mapping::shared_ptr> mappings;

        QString name;
    };
}
