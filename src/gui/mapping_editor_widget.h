#pragma once

#include <QWidget>
#include <QTableWidget>
#include <QHBoxLayout>

#include "mapping.h"
#include "player_widget.h"

namespace mechanizm {

    class MappingEditorWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<MappingEditorWidget> unique_ptr;

        explicit MappingEditorWidget(QWidget *parent = 0);
        virtual ~MappingEditorWidget() {};

        void setMapping(mechanizm::Mapping::shared_ptr c);
        void onClipCellSelected(int row, int column);

    protected:
        virtual void keyPressEvent(QKeyEvent *event) override;

    private:
        void updateClipTable();
        void updateSequenceTable();
        
        mechanizm::Mapping::shared_ptr mapping;

        std::unique_ptr<QTableWidget> clipTable;
        std::unique_ptr<QTableWidget> sequenceTable;
        std::unique_ptr<QHBoxLayout> hbox;
        std::unique_ptr<mechanizm::PlayerWidget> player;
    };
}
