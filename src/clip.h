#pragma once

#include <list>
#include <string>

#include <Clip.h>
#include <Fraction.h>
#include <KeyFrame.h>

namespace mechanizm {
    class Clip : public openshot::Clip {
    public:
        typedef std::shared_ptr<Clip> shared_ptr;

        Clip() : openshot::Clip() {
        };
        
        Clip(std::string p) 
                : openshot::Clip(p) {
        };

        Clip (openshot::ReaderBase *r)
                : openshot::Clip(r) {
        };

        virtual ~Clip() {};

        void mapTimeToBpm(int bpm) {
            openshot::Fraction fps = this->Reader()->info.fps;
            int quarterNoteLength = (60 * fps.ToFloat()) / bpm;
            int pos = 0;
            for(auto p = points.begin(); p != points.end(); p++ ) {
                openshot::Point newPoint(pos, p->co.X); //, openshot::InterpolationType::LINEAR);
                this->time.AddPoint(newPoint);
                pos += quarterNoteLength;
            }
            this->time.UpdatePoint(0, this->time.GetPoint(1));

            this->time.PrintValues();
            this->time.PrintPoints();
            this->Start(0);
            this->End(pos / fps.ToFloat());
        }


        void addPoint(openshot::Point p) {    
            points.push_back(p);
        }
        

        private: 
            std::list<openshot::Point> points;

    };  
}