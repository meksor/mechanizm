#pragma once

#include <memory>

#include <OpenShot.h>

#include <QWidget>
#include <QKeyEvent>

#include "clip.h"
#include "project.h"
#include "player.h"

namespace mechanizm {
    class EditorWindow : public mechanizm::PlayerWindow {
        Q_OBJECT
    public:
        typedef std::shared_ptr<EditorWindow> shared_ptr;

        explicit EditorWindow(QWidget *parent = 0);
        virtual ~EditorWindow() {};

        void setClip(mechanizm::Clip::shared_ptr c);
        void setInPoint(long long position);
        void setOutPoint(long long position);
        void setRythmicBasePoint(long long position);

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    private:
        mechanizm::Clip::shared_ptr clip;
    };
}