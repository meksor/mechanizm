#pragma once
#include <string>
#include <list>
#include <memory>

#include <OpenShot.h>
#include <Clip.h>
#include <Timeline.h>
#include "clip.h"
#include "reader.h"

namespace mechanizm {   
    class Project {
    public:
        typedef std::shared_ptr<Project> shared_ptr;

        Project();
        virtual ~Project() {};

        int getFps() { return fps; };
        int getWidth() { return width; };
        int getHeight() { return height; };

        std::list<mechanizm::Clip::shared_ptr> getClips() { return clips; };
        std::list<mechanizm::FFmpegReader::shared_ptr> getReaders() { return readers; };

        void addFile(std::string path);
        void addClip(mechanizm::FFmpegReader::shared_ptr r);

    private:
        std::list<mechanizm::FFmpegReader::shared_ptr> readers;
        std::list<mechanizm::Clip::shared_ptr> clips;
        int width = 1080;
        int height = 720;
        int fps = 30;
    };
}
