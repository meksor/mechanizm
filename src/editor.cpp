#include <Point.h>
#include "editor.h"

namespace mechanizm {   

    /*
        EditorWindow
    */

    EditorWindow::EditorWindow(QWidget *parent) : mechanizm::PlayerWindow(parent) {
        this->setWindowTitle("Editor");
    }

    void EditorWindow::keyPressEvent(QKeyEvent *event) {
        mechanizm::PlayerWindow::keyPressEvent(event);
        switch (event->key()) {
        case Qt::Key_I:
            setInPoint(player->Position());
            break;
        case Qt::Key_O:
            setOutPoint(player->Position());
            break;
        case Qt::Key_R:
            setRythmicBasePoint(player->Position());
            break;
        default:
            break;
        }
    }

    void EditorWindow::setClip(mechanizm::Clip::shared_ptr c) {
        clip = c;
        this->setReader(clip->Reader());
    }

    void EditorWindow::setInPoint(long long position) {
        setRythmicBasePoint(position);
    }

    void EditorWindow::setOutPoint(long long position) {
        setRythmicBasePoint(position);
    }
    
    void EditorWindow::setRythmicBasePoint(long long position) {
        float time = ((float)position) / clip->Reader()->info.fps.num;
        openshot::Point point(position, position);
        clip->addPoint(point);
    }
}
