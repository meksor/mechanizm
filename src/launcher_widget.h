#pragma once

#include <memory>

#include <QHBoxLayout>
#include <QObject>
#include <QFileDialog>
#include <QWidget>
#include <QPushButton>
#include <QString>

namespace mechanizm {

    class LauncherWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<LauncherWidget> unique_ptr;

        explicit LauncherWidget(QWidget *parent = 0);
        virtual ~LauncherWidget() {};

        void onNewProjectClicked(bool checked);
        void onOpenProjectClicked(bool checked);

    signals:
        void openProjectDirectory(QString dirPath);
        void newProject(QString name, QString dirPath);

    private:
        std::unique_ptr<QHBoxLayout> hbox;
        std::unique_ptr<QPushButton> newProjectButton;
        std::unique_ptr<QPushButton> openProjectButton;
    };
    
}