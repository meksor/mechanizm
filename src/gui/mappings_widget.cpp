#include "mappings_widget.h"

namespace mechanizm {
    MappingsWidget::MappingsWidget(QWidget *parent) : QWidget(parent) {
        hbox = std::make_unique<QHBoxLayout>(this);
        list = std::make_unique<QListWidget>();
        list->setFocusPolicy(Qt::NoFocus);

        this->setFixedSize(480, 480);
        hbox->addWidget(list.get());
        setLayout(hbox.get());
    }

    void MappingsWidget::setMappings(std::vector<mechanizm::Mapping::shared_ptr> c) {
        mappings = c;
        updateList();
    };

    void MappingsWidget::updateList() {
        list->clear();
        for (auto mapping : mappings) {
            QString name = mapping->getName();
            list->addItem(name);
        }
    }

    mechanizm::Mapping::shared_ptr MappingsWidget::currentMapping() {
        return mappings[list->currentRow()];
    }

    void MappingsWidget::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
        case Qt::Key_E:
            event->accept();
            emit mappingEditorRequested(currentMapping());
            break;
        default:
            QWidget::keyPressEvent(event);
            break;
        }
    }

}
