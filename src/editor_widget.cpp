#include "editor_widget.h"
#include <QDebug>
#include <QHeaderView>

namespace mechanizm {
    EditorWidget::EditorWidget(QWidget *parent) : QWidget(parent) {
        hbox = std::make_unique<QHBoxLayout>(this);
        table = std::make_unique<QTableWidget>();
        table->setColumnCount(2);
        table->setHorizontalHeaderItem(0, new QTableWidgetItem("Frame #"));
        table->setHorizontalHeaderItem(1, new QTableWidgetItem("Timestamp"));
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        player = std::make_unique<mechanizm::PlayerWidget>();
        this->setFixedSize(900, 480);

        hbox->addWidget(player.get());
        hbox->addWidget(table.get());
        setLayout(hbox.get());
    }

    void EditorWidget::setClip(mechanizm::Clip::shared_ptr c) {
        clip = c;
        player->setReader(clip->getProxyReader().get());
        updateTable();
    };

    void EditorWidget::updateTable() {
        table->clearContents();
        auto points = clip->getRythmicPoints();
        int count = points.GetCount();
        table->setRowCount(count);
        for (int i=0; i<count; i++) {
            auto point = points.GetPoint(i);
            QTableWidgetItem *frameItem = new QTableWidgetItem(tr("%1").arg(point.co.X));
            QTableWidgetItem *timestampItem = new QTableWidgetItem(tr("%1s").arg(point.co.Y));
            table->setItem(i, 0, frameItem);
            table->setItem(i, 1, timestampItem);
        }
    }


    void EditorWidget::showEvent(QShowEvent *event) {
        player->setFocus();
    }

    void EditorWidget::keyPressEvent(QKeyEvent *event) {
        qDebug() << event;
        long frame = player->getPlayer()->Position();
        openshot::Fraction fps = clip->getProxyReader()->info.fps;
        float timestamp = frame / fps.ToFloat();

        switch (event->key()) {
        case Qt::Key_R:
            clip->addRythmicPoint(frame, timestamp);
            updateTable();
            break;
        default:
            event->ignore();
            break;
        }
    }

}
