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

    class Source : public mechanizm::JsonSerializable {
    public:
        typedef std::shared_ptr<Source> shared_ptr;

        Source(QFile &importFile, QDir &projectDir);
        Source(QString dirPath);
        virtual ~Source() {};

        QString getRelativePath(QDir dir);

        QString getFilePath() { return rootDir.absoluteFilePath(filePath); }
        QString getProxyPath() { return rootDir.absoluteFilePath(proxyPath); }
        QString getFileHash() { return fileHash; }

        Json::Value JsonValue () const override;
        void SetJsonValue (const Json::Value root) override;

    protected:
        void saveToDisk();

    private:        
        void makeProxyMedia();

        QString name;
        QDir rootDir;
        QString fileHash;
        QString filePath;
        QString proxyPath;
    };

}
