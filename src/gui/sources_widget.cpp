#include <QFileDialog>

#include "sources_widget.h"

namespace mechanizm {

    SourcesWidget::SourcesWidget(QWidget *parent) : QWidget(parent) {
        hbox = std::make_unique<QHBoxLayout>(this);
        list = std::make_unique<QListWidget>();
        list->setFocusPolicy(Qt::NoFocus);

        proofPlayer = std::make_unique<mechanizm::PlayerWidget>();

        this->setFixedSize(480, 480);
      
        hbox->addWidget(list.get());
        setLayout(hbox.get());
    }

    void SourcesWidget::setSources(std::vector<mechanizm::Source::shared_ptr> s) {
        sources = s;
        updateList();
    }
    
    mechanizm::Source::shared_ptr SourcesWidget::currentSource() {
        return sources[list->currentRow()];
    }

    void SourcesWidget::updateList() {
        list->clear();
        for (auto it = sources.begin(); it != sources.end(); ++it) {
            QString name = (*it)->getFileHash();
            list->addItem(name);
        }
    }
    
    void SourcesWidget::openImportDialogue() {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("Videos (*.mp4)"));
        int result = dialog.exec();
        QString filePath;
        mechanizm::Source::shared_ptr source;
        if (result) {
            filePath = dialog.selectedFiles()[0];
            emit importFileSelected(filePath);
        }
    }

    void SourcesWidget::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
        case Qt::Key_I:
            this->openImportDialogue();
            break;
        case Qt::Key_N:
            emit newClip(currentSource());
            break;
        case Qt::Key_P:
            if (!proofPlayer->isVisible()) {
                proofPlayer->readFile(currentSource()->getFilePath());
                proofPlayer->show();
            } else {
                proofPlayer->close();
            }
            break;
        default:
            QWidget::keyPressEvent(event);
            break;
        }
    }

}