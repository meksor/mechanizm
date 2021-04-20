#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QHBoxLayout>

#include "clip.h"
#include "player_widget.h"

namespace mechanizm {

    class EditorWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<EditorWidget> unique_ptr;

        explicit EditorWidget(QWidget *parent = 0);
        virtual ~EditorWidget() {};

        void setClip(mechanizm::Clip::shared_ptr c);

    protected:
        virtual void keyPressEvent(QKeyEvent *event) override;

    private:
        void updateTable();
        void setRythmicPoint();
   
        mechanizm::Clip::shared_ptr clip;

        std::unique_ptr<QTableWidget> table;
        std::unique_ptr<QHBoxLayout> hbox;
        std::unique_ptr<mechanizm::PlayerWidget> player;
    };

}