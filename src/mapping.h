#pragma once
#include <memory>
#include <QDir>
#include <QString>

#include <Json.h>

#include "json.h"
#include "clip.h"
#include "sequence.h"


namespace mechanizm {
    class Mapping : public mechanizm::JsonStorable {
    public:
        typedef std::shared_ptr<Mapping> shared_ptr;

        Mapping(mechanizm::Clip::shared_ptr clip, mechanizm::Sequence::shared_ptr sequence, QString name, QDir &projectDir);
        Mapping(QDir &rootDir);
        virtual ~Mapping() {};
        
        void map();

        void setName(QString n) { name = n; };
        QString getName() const { return name; };
        std::shared_ptr<openshot::Clip> getMappedClip() { return mappedClip; };

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;

        mechanizm::Clip::shared_ptr clip;  // TODO: get/set
        mechanizm::Sequence::shared_ptr sequence; // TODO: get/set

    protected:
        virtual QString getDirectoryPath() const override { return "mappings/" + name; };
        virtual QString getJsonFileName() const override { return "mapping.json"; };

    private:
        QString name;        
        std::shared_ptr<openshot::Clip> mappedClip;

    };
}
