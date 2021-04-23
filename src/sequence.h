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
        double getT() { return t; };

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;

    private:
        double t; // 1. == 1 Quater Note
    };

    class Sequence : public std::vector<SequenceNode>, public mechanizm::JsonStorable {
    public:
        typedef std::shared_ptr<Sequence> shared_ptr;

        Sequence();
        Sequence(QFile &midiFile, unsigned int trackIndex, QDir &projectDir);
        Sequence(QDir &rootDir);
        virtual ~Sequence() {};

        void addNode(Json::Value jsonNode);
        double getBpm() { return bpm; };
        double getMaxT() { return maxT; };
        double getT(int i);

        void setName(QString n) { name = n; };
        QString getName() const { return name; };

        Json::Value JsonValue() const override;
        void SetJsonValue(const Json::Value root) override;


    protected:
        virtual QString getDirectoryPath() const override { return "sequences/" + fileHash; };
        virtual QString getJsonFileName() const override { return "sequence.json"; };

    private:
        double bpm;
        double timeSignature;
        double maxT;

        QString name;
        QString fileHash;
    };
    
}
