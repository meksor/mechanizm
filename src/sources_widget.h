#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QListWidget>
#include <QHBoxLayout>

#include "source.h"
#include "player_widget.h"

namespace mechanizm {

    class SourcesWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<SourcesWidget> unique_ptr;

        explicit SourcesWidget(QWidget *parent = 0);
        virtual ~SourcesWidget() {};

        void setSources(std::vector<mechanizm::Source::shared_ptr> s);
        
        mechanizm::Source::shared_ptr currentSource();

    signals:
        void importFileSelected(QString filePath);
        void newClip(mechanizm::Source::shared_ptr source);

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    private:
        void updateList();
        void openImportDialogue();

        std::vector<mechanizm::Source::shared_ptr> sources;
        mechanizm::PlayerWidget::unique_ptr proofPlayer;

        std::unique_ptr<QListWidget> list;
        std::unique_ptr<QHBoxLayout> hbox;
    };

} 
