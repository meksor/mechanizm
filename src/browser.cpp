#include <iterator>
#include <iostream>
#include <FFmpegWriter.h>
#include "browser.h"

namespace mechanizm {   

    /*
        ProjectListWidget
    */

    ProjectListWidget::ProjectListWidget(QWidget *parent)
            : QListWidget(parent) {
    }
    void ProjectListWidget::setProject(mechanizm::Project::shared_ptr p) {
        project = p;
        this->updateItems();
    }

    /*
        ProjectReaderListWidget
    */

    ProjectReaderListWidget::ProjectReaderListWidget(QWidget *parent)
            : ProjectListWidget(parent) {
    }

    void ProjectReaderListWidget::updateItems() {
        this->clear();
        auto readers = this->project->getReaders();
        for (auto it = readers.begin() ; it != readers.end(); ++it) {
            std::string name = (*it)->getName();
            this->addItem(QString::fromUtf8(name.c_str()));
        }
    }
    mechanizm::FFmpegReader::shared_ptr ProjectReaderListWidget::currentReader() {
        auto readers = this->project->getReaders();
        auto it = readers.begin();
        std::advance(it, this->currentRow());
        return std::shared_ptr<mechanizm::FFmpegReader>(
            *it
        );
    }

    /*
        ProjectClipListWidget
    */

    ProjectClipListWidget::ProjectClipListWidget(QWidget *parent)
            : ProjectListWidget(parent) {
    }

    void ProjectClipListWidget::updateItems() {
        this->clear();
        auto clips = this->project->getClips();
        int idx = 0;
        for (auto it = clips.begin() ; it != clips.end(); ++it) {
            std::string name = "Clip " + std::to_string(idx);
            this->addItem(QString::fromUtf8(name.c_str()));
            idx++;
        }
    }
    mechanizm::Clip::shared_ptr ProjectClipListWidget::currentClip() {
        auto clips = this->project->getClips();
        auto it = clips.begin();
        std::advance(it, this->currentRow());
        return mechanizm::Clip::shared_ptr(
            *it
        );
    }


    /*
        BrowserWindow
    */

    BrowserWindow::BrowserWindow(QWidget *parent) : QWidget(parent) {
        QHBoxLayout *hbox = new QHBoxLayout(this); // TODO: Fix this memory leak

        clipList = std::make_unique<ProjectClipListWidget>(this);
        readerList = std::make_unique<ProjectReaderListWidget>(this);
        this->setFixedSize(720, 480);
      
        hbox->addWidget(clipList.get());
        hbox->addWidget(readerList.get());
        setLayout(hbox);
    }

    void BrowserWindow::keyPressEvent(QKeyEvent *event) {
        std::cout << event->key() << std::endl;
        switch (event->key()) {
        case Qt::Key_E:
            // new editor window
            openEditorWindow(clipList->currentClip());
            break;
        case Qt::Key_N:
            project->addClip(readerList->currentReader());
            clipList->updateItems();
            // New Clip from file
            break;
        case Qt::Key_P:
            openPlayerWindow(clipList->currentClip());
            // Player window
            break;
        default:
            break;
        }
    }

    void BrowserWindow::setProject(mechanizm::Project::shared_ptr p) {
        project = p;
        readerList->setProject(p);
        clipList->setProject(p);
    }

    void BrowserWindow::openEditorWindow(mechanizm::Clip::shared_ptr c) {
        mechanizm::EditorWindow *window = new mechanizm::EditorWindow(); // TODO: fix this memory leak
        window->setClip(c);
        window->show();
    }

    void BrowserWindow::openPlayerWindow(mechanizm::Clip::shared_ptr c) {
        mechanizm::PlayerWindow *window = new mechanizm::PlayerWindow(); // TODO: fix this memory leak
        auto t = new openshot::Timeline(
            c->Reader()->info.width,
            c->Reader()->info.height,
            c->Reader()->info.fps, // framerate
            c->Reader()->info.sample_rate, // sample rate
            c->Reader()->info.channels, // channels
            c->Reader()->info.channel_layout
        );
        c->mapTimeToBpm(120);
        t->AddClip(c.get());
        t->Open();
        window->setReader(t);
        window->show();

        openshot::FFmpegWriter writer("./test.mp4");
        writer.SetVideoOptions(true, "libx264", openshot::Fraction(25,1), 720, 480, openshot::Fraction(1,1), false, false, 300000);
        writer.Open();
        writer.WriteFrame(t, 0, t->GetMaxFrame());
        writer.Close();
    }
}