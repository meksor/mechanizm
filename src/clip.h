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

    class Clip : public openshot::Clip, public mechanizm::JsonSerializable {

    using mechanizm::JsonSerializable::SetJson;
    using mechanizm::JsonSerializable::Json;

    public:
        typedef std::shared_ptr<Clip> shared_ptr;

        Clip(QString name, mechanizm::Source::shared_ptr source, QDir &projectDir);
        Clip(QString dirPath);

        virtual ~Clip() {};

        void setName(QString n) { name = n; };
        QString getName() { return name; };
        QString getRelativePath(QDir dir);
        std::shared_ptr<openshot::FFmpegReader> getProxyReader() { return proxyReader; };

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;

        void addRythmicPoint(long x, float y);
        openshot::Keyframe getRythmicPoints() { return rythmicPoints; };
        
    protected:
        void saveToDisk();
        openshot::Keyframe rythmicPoints;

    private:
        void initReader();

        QDir rootDir;
        QString name;
        mechanizm::Source::shared_ptr source;
        std::shared_ptr<openshot::FFmpegReader> proxyReader;
    };

}
