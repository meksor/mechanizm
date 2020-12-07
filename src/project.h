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
        void importFile(QString filePath) ;
        void newClip(mechanizm::Source::shared_ptr source);        
        void addSource(QString dirPath);
        void addClip(QString dirPath);
    
    signals:
        void sourcesChanged(std::vector<mechanizm::Source::shared_ptr> sources);
        void clipsChanged(std::vector<mechanizm::Clip::shared_ptr> sources);

    private: 
        std::vector<mechanizm::Source::shared_ptr> sources;
        std::vector<mechanizm::Clip::shared_ptr> clips;
        QDir rootDir;
        QString name;
    };
}
