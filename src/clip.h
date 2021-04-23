#pragma once

#include <memory>
#include <string>

#include <QString>
#include <QDir>

#include <Clip.h>
#include <KeyFrame.h>
#include <FFmpegReader.h>

#include "json.h"
#include "source.h"

namespace mechanizm {

    class Clip 
    : public openshot::Clip, public mechanizm::JsonStorable {
    
    public:
        typedef std::shared_ptr<Clip> shared_ptr;

        Clip(QString name, mechanizm::Source::shared_ptr source, QDir &projectDir);
        Clip(QDir &rootDir);
        virtual ~Clip() {};

        void setName(QString n) { name = n; };
        QString getName() const { return name; };
        std::shared_ptr<openshot::FFmpegReader> getProxyReader() const { return proxyReader; };

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;


        void addRythmicPoint(long x, float y);
        openshot::Keyframe getRythmicPoints() { return rythmicPoints; };
        
    protected:
        virtual QString getDirectoryPath() const override { return "clips/" + name; };
        virtual QString getJsonFileName() const override { return "clip.json"; };

        openshot::Keyframe rythmicPoints;

    private:
        void initReader();

        QString name;
        mechanizm::Source::shared_ptr source;
        std::shared_ptr<openshot::FFmpegReader> proxyReader;
    };

}
