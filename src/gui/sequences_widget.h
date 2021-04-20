#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QListWidget>
#include <QHBoxLayout>

#include "sequence.h"

namespace mechanizm {

    class SequencesWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<SequencesWidget> unique_ptr;

        explicit SequencesWidget(QWidget *parent = 0);
        virtual ~SequencesWidget() {};

        void setSequences(std::vector<mechanizm::Sequence::shared_ptr> s);
        
        mechanizm::Sequence::shared_ptr currentSource();

    signals:
        void importFileSelected(QString filePath);

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    private:
        void updateList();
        void openImportDialogue();

        std::vector<mechanizm::Sequence::shared_ptr> sequences;

        std::unique_ptr<QListWidget> list;
        std::unique_ptr<QHBoxLayout> hbox;
    };

} 
