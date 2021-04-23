#include "mapping_editor_widget.h"
#include <QHeaderView>
#include <Timeline.h>
#include <FFmpegWriter.h>
#include <ZmqLogger.h>

namespace mechanizm {
    MappingEditorWidget::MappingEditorWidget(QWidget *parent) : QWidget(parent) {
        hbox = std::make_unique<QHBoxLayout>(this);

        clipTable = std::make_unique<QTableWidget>();
        clipTable->setColumnCount(2);
        clipTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Frame #"));
        clipTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Timestamp"));
        clipTable->setFocusPolicy(Qt::NoFocus);
        clipTable->setEditTriggers(QAbstractItemView::DoubleClicked);
        clipTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        clipTable->setSelectionMode(QAbstractItemView::SingleSelection);
        clipTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        QObject::connect(clipTable.get(), &QTableWidget::cellClicked,
            this, &mechanizm::MappingEditorWidget::onClipCellSelected);

        sequenceTable = std::make_unique<QTableWidget>();
        sequenceTable->setColumnCount(1);
        sequenceTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Quarter Notes Offset"));
        sequenceTable->setFocusPolicy(Qt::NoFocus);
        sequenceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        sequenceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        sequenceTable->setSelectionMode(QAbstractItemView::SingleSelection);
        sequenceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        player = std::make_unique<mechanizm::PlayerWidget>();
        hbox->addWidget(player.get());
        hbox->addWidget(clipTable.get());
        hbox->addWidget(sequenceTable.get());

        this->setFixedSize(1000, 500);
        this->setLayout(hbox.get());
    }

    void MappingEditorWidget::setMapping(mechanizm::Mapping::shared_ptr m) {
        mapping = m;
        auto c = mapping->getMappedClip();

        auto t = new openshot::Timeline(
            c->Reader()->info.width,
            c->Reader()->info.height,
            c->Reader()->info.fps,
            c->Reader()->info.sample_rate,
            c->Reader()->info.channels,
            c->Reader()->info.channel_layout        
        );
        t->AddClip(c.get());
        t->Open();
        player->setReader(t);

        openshot::FFmpegWriter writer("./test.mp4");
        writer.SetVideoOptions(
            true, "libx264", 
            c->Reader()->info.fps,
            c->Reader()->info.width,
            c->Reader()->info.height,
            c->Reader()->info.pixel_ratio,
            false, false, 300000);

        writer.Open();
        int maxFrame = t->GetMaxFrame();
        writer.WriteFrame(t, 0, maxFrame);
        writer.Close();

        player->getPlayer()->Stop();
        player->getPlayer()->Seek(0);
        player->getPlayer()->Play();
        
        updateClipTable();
        updateSequenceTable();
    }

    void MappingEditorWidget::onClipCellSelected(int row, int column) {
        //auto points = mapping->clip->getRythmicPoints();
        //auto point = points.GetPoint(row);
        //player->getPlayer()->Pause();
        //player->getPlayer()->Seek(point.co.X);
    }

    void MappingEditorWidget::updateClipTable() {
        clipTable->clearContents();
        auto points = mapping->clip->getRythmicPoints();
        int count = points.GetCount();
        clipTable->setRowCount(count);
        for (int i = 0; i < count; i++) {
            auto point = points.GetPoint(i);
            QTableWidgetItem *frameItem = new QTableWidgetItem(tr("%1").arg(point.co.X));
            QTableWidgetItem *timestampItem = new QTableWidgetItem(tr("%1s").arg(point.co.Y));
            clipTable->setItem(i, 0, frameItem);
            clipTable->setItem(i, 1, timestampItem);
        }
    }

    void MappingEditorWidget::updateSequenceTable() {
        sequenceTable->clearContents();
        int count = mapping->sequence->size();
        sequenceTable->setRowCount(count);
        for (int i = 0; i < count; i++) {
            auto node = mapping->sequence->at(i);
            QTableWidgetItem *tItem = new QTableWidgetItem(tr("%1qn").arg(node.getT()));
            sequenceTable->setItem(i, 0, tItem);
        }
    }

    void MappingEditorWidget::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
        default:
            player->keyPressEvent(event);
            break;
        }
    }
}

