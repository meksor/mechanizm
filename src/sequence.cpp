#include <cmath>
#include <QCryptographicHash>
#include <cxxmidi/file.hpp>
#include "sequence.h"

#define MIDI_NOTE_ON 0x90

namespace mechanizm {

    SequenceNode::SequenceNode() {}
    SequenceNode::SequenceNode(double t): t(t) {}

    double SequenceNode::getTimestamp(double bpm) {
        return t / (bpm / 60.);
    }

    Json::Value SequenceNode::JsonValue() const {
        Json::Value root;
        root["t"] = t;

        return root;
    }

    void SequenceNode::SetJsonValue(const Json::Value root) {
        t = root["t"].asDouble();
    }

    Sequence::Sequence() {
        bpm = 120.;
        timeSignature = 1.;
        name = "";
    }

    Sequence::Sequence(QFile &midiFile, unsigned int trackIndex, QDir &projectDir) {
        if (midiFile.open(QIODevice::ReadOnly)) {
            QByteArray fileData = midiFile.readAll();
            fileHash = QCryptographicHash::hash(fileData, QCryptographicHash::Md5).toHex();
        } 
        
        name = midiFile.fileName().mid(midiFile.fileName().lastIndexOf("/"));
        mechanizm::JsonStorable::create(projectDir);

        bpm = 120.;
        timeSignature = 1.;

        cxxmidi::File file(midiFile.fileName().toStdString().c_str());
        cxxmidi::Track track = file[trackIndex];

        double totalT = 0.;
        for (auto ev : track) {
            totalT += ev.Dt();
        }

        double totalQuarterNotes = (totalT / file.TimeDivision());
        double qnPerMeasure = 4. * timeSignature;
        double padding = qnPerMeasure - std::fmod(totalQuarterNotes, qnPerMeasure);
        maxT = totalQuarterNotes + padding;

        double currentT = 0.;
        for (auto ev : track) {
            currentT += ev.Dt();
            if (ev[0] == MIDI_NOTE_ON) {
                SequenceNode node(currentT / file.TimeDivision());
                this->push_back(node);
            }
        }
        mechanizm::JsonStorable::saveToDisk();
    }

    Sequence::Sequence(QDir &rootDir) {
        mechanizm::JsonStorable::loadFromDisk(rootDir);
    };

    void Sequence::addNode(Json::Value jsonNode) {
        SequenceNode node;
        node.SetJsonValue(jsonNode);
        this->push_back(node);
    }

    double Sequence::getT(int i) {
        mechanizm::SequenceNode node = this->at(i % this->size());
        double offs = floor(i / this->size() * maxT);
        return offs + node.getT();
    }

    Json::Value Sequence::JsonValue() const {
        Json::Value root;
        root["name"] = name.toStdString();
        root["bpm"] = bpm;
        root["timeSignature"] = timeSignature;
        root["maxT"] = maxT;

        Json::Value &nodes = root["nodes"];
        for (int i = 0; i < this->size(); ++i )
            nodes[i] = this->at(i).JsonValue();

        return root;
    }

    void Sequence::SetJsonValue(const Json::Value root) {
        name = root["name"].asCString();
        bpm = root["bpm"].asDouble();
        timeSignature = root["timeSignature"].asDouble();
        maxT = root["maxT"].asDouble();

        const Json::Value nodes = root["nodes"];
        for (int i = 0; i < nodes.size(); ++i )
            addNode(nodes[i]);

    }

}