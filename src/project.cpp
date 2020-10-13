#include <Fraction.h>

#include "project.h"

namespace mechanizm {   

    /*
        Project
    */

    Project::Project() {

    }

    void Project::addFile(std::string path) {
        auto reader = std::make_shared<mechanizm::FFmpegReader>(path);
        readers.push_back(reader); 
    }
    
    void Project::addClip(mechanizm::FFmpegReader::shared_ptr r) {
        auto clip = std::make_shared<mechanizm::Clip>(r.get());
        clips.push_back(clip);
    }

}