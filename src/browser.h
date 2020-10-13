#pragma once

#include <memory>
#include <vector>

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "project.h"
#include "clip.h"
#include "editor.h"

namespace mechanizm {

    class ProjectListWidget : public QListWidget {
        Q_OBJECT
    public:
        explicit ProjectListWidget(QWidget *parent = 0);
        virtual ~ProjectListWidget() {};

        void setProject(mechanizm::Project::shared_ptr p);
        virtual void updateItems() = 0;
    protected:
        mechanizm::Project::shared_ptr project;
    };


    class ProjectReaderListWidget : public ProjectListWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<ProjectReaderListWidget> unique_ptr;

        explicit ProjectReaderListWidget(QWidget *parent = 0);
        virtual ~ProjectReaderListWidget() {};

        void updateItems() override;
        mechanizm::FFmpegReader::shared_ptr currentReader();
    };

    class ProjectClipListWidget : public ProjectListWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<ProjectClipListWidget> unique_ptr;

        explicit ProjectClipListWidget(QWidget *parent = 0);
        virtual ~ProjectClipListWidget() {};

        void updateItems() override;
        mechanizm::Clip::shared_ptr currentClip();
    };


    class BrowserWindow : public QWidget {
        Q_OBJECT
    public:
        typedef std::shared_ptr<BrowserWindow> shared_ptr;

        explicit BrowserWindow(QWidget *parent = 0);
        virtual ~BrowserWindow() {};

        void setProject(mechanizm::Project::shared_ptr p);
        void openEditorWindow(mechanizm::Clip::shared_ptr c);
        void openPlayerWindow(mechanizm::Clip::shared_ptr c);

    protected:
        void keyPressEvent(QKeyEvent *event) override;

    private:
        mechanizm::Project::shared_ptr project;
        ProjectReaderListWidget::unique_ptr readerList;
        ProjectClipListWidget::unique_ptr clipList;
    };

}
