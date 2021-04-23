#pragma once

#include <memory>
#include <exception>

#include <QDir>
#include <QFile>
#include <QString>

#include <Json.h>

#include "json.h"

namespace mechanizm {

    struct SourceExists : public std::exception {
        const char * what () const throw () {
            return "Source already exists";
        }
    };

    struct InvalidSourceDirectory : public std::exception {
        const char * what () const throw () {
            return "Directory is not a mechanizm source directory";
        }
    };

    class Source : public mechanizm::JsonStorable {
    public:
        typedef std::shared_ptr<Source> shared_ptr;

        Source(QFile &importFile, QDir &projectDir);
        Source(QDir &rootDir);
        virtual ~Source() {};

        QString getFilePath() { return rootDir.absoluteFilePath(filePath); }
        QString getProxyPath() { return rootDir.absoluteFilePath(proxyPath); }
        QString getFileHash() { return fileHash; }

        Json::Value JsonValue() const override;
        void SetJsonValue (const Json::Value root) override;

    protected:
        virtual QString getDirectoryPath() const override { return "sources/" + fileHash; };
        virtual QString getJsonFileName() const override { return "source.json"; };
        virtual void setupDirectory() override;

    private:        
        void makeProxyMedia();

        QString name;
        QString fileHash;
        QString filePath;
        QString proxyPath;
    };

}
