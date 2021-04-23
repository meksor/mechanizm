#include <algorithm> 
#include <math.h>      
#include "mapping.h"

namespace mechanizm {
    Mapping::Mapping(
        mechanizm::Clip::shared_ptr clip, 
        mechanizm::Sequence::shared_ptr sequence, 
        QString name, 
        QDir &projectDir
    ) : clip(clip), sequence(sequence), name(name) {
            JsonStorable::create(projectDir);
            mappedClip = std::make_shared<openshot::Clip>();
            mappedClip->SetJsonValue(clip->JsonValue());
            mappedClip->Reader(clip->getProxyReader().get());
            map();
            JsonStorable::saveToDisk();
        }

    Mapping::Mapping(QDir &rootDir) {
        JsonStorable::loadFromDisk(rootDir);
    }

    void Mapping::map() {
        openshot::Fraction fps = this->mappedClip->Reader()->info.fps;
        double quarterNoteLength = (60. * fps.ToFloat()) / sequence->getBpm();
        auto points = this->clip->getRythmicPoints();
        int max = std::max(points.GetCount(), (int64_t)sequence->size());

        double pos = 0.;
        for(int i = 0; i < max; i++) {
            openshot::Point point = points.GetPoint(i % points.GetCount());
            pos = sequence->getT(i);
            openshot::Point newPoint(round(quarterNoteLength * pos), point.co.X);
            this->mappedClip->time.AddPoint(newPoint);
        }
        this->mappedClip->Start(sequence->getT(1) / 2.);
        this->mappedClip->End(pos / 2.);
        this->mappedClip->time.UpdatePoint(0, this->mappedClip->time.GetPoint(1));

    }

    Json::Value Mapping::JsonValue() const {
        Json::Value root;
        root["name"] = name.toStdString();
        root["clip"] = clip->getRelativePath(rootDir).toStdString();
        root["sequence"] = sequence->getRelativePath(rootDir).toStdString();

        root["mappedClip"] = mappedClip->JsonValue();

        return root;
    }

    void Mapping::SetJsonValue(const Json::Value root) {
        name = root["name"].asCString();

        QString clipPath = root["clip"].asCString();
        QDir clipDir(rootDir.filePath(clipPath));
        if (clipDir.exists()) {
            clip = std::make_shared<mechanizm::Clip>(clipDir);
        } // TODO: Error

        QString sequencePath = root["sequence"].asCString();
        QDir sequenceDir(rootDir.filePath(sequencePath));
        if (sequenceDir.exists()) {
            sequence = std::make_shared<mechanizm::Sequence>(sequenceDir);
        } // TODO: Error

        mappedClip = std::make_shared<openshot::Clip>();
        mappedClip->SetJsonValue(root["mappedClip"]);
    }
}
