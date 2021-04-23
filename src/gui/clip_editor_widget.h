#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QHBoxLayout>

#include "clip.h"
#include "player_widget.h"

namespace mechanizm {

    class ClipEditorWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<ClipEditorWidget> unique_ptr;

        explicit ClipEditorWidget(QWidget *parent = 0);
        virtual ~ClipEditorWidget() {};

        void setClip(mechanizm::Clip::shared_ptr c);
        void onCellSelected(int row, int column);

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
