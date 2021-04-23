#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QListWidget>
#include <QHBoxLayout>

#include "clip.h"

namespace mechanizm {

    class ClipsWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<ClipsWidget> unique_ptr;

        explicit ClipsWidget(QWidget *parent = 0);
        virtual ~ClipsWidget() {};

        void setClips(std::vector<mechanizm::Clip::shared_ptr> c);
        mechanizm::Clip::shared_ptr currentClip();

    protected:
        virtual void keyPressEvent(QKeyEvent *event) override;

    signals:
        void clipEditorRequested(mechanizm::Clip::shared_ptr c);

    private:
        void updateList();
   
        std::vector<mechanizm::Clip::shared_ptr> clips;

        std::unique_ptr<QListWidget> list;
        std::unique_ptr<QHBoxLayout> hbox;
    };

}
