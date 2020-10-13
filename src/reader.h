#pragma once

#include <string>
#include <FFmpegReader.h>

namespace mechanizm {
    class FFmpegReader : public openshot::FFmpegReader {
    public:
        typedef std::shared_ptr<FFmpegReader> shared_ptr;

        FFmpegReader(std::string p) 
            : openshot::FFmpegReader(p) {
                path = p;
            };
        FFmpegReader(std::string p, bool inspect_reader) 
            : openshot::FFmpegReader(p, inspect_reader) {
                path = p;
            };

        virtual ~FFmpegReader() {};

        std::string getName() {
            return path;
        };

    private:
        std::string path;
    };
}