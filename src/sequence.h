#pragma once
#include <memory>
#include <vector>

#include <QString>
#include <QFile>

#include <Json.h>

#include "json.h"


namespace mechanizm {
    class SequenceNode : public mechanizm::JsonSerializable {
    public:
        typedef std::shared_ptr<SequenceNode> shared_ptr;

        SequenceNode();
        SequenceNode(double t);
        virtual ~SequenceNode() {};
        
        double getTimestamp(double bpm);

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;

    private: 
        double t; // 1. == 1 Quater Note
    };

    class Sequence : public std::vector<SequenceNode>, public mechanizm::JsonSerializable {
    public:
        typedef std::shared_ptr<Sequence> shared_ptr;


        Sequence();
        Sequence(QFile &midiFile, unsigned int trackIndex);
        Sequence(double bpm, double timeSignature, double maxT, QString name);
        virtual ~Sequence() {};

        void addNode(Json::Value jsonNode);

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;

        QString name;

    private:
        double bpm;
        double timeSignature;
        double maxT;

    };
    
}
