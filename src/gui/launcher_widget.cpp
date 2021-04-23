#include "launcher_widget.h"

namespace mechanizm {
    LauncherWidget::LauncherWidget(QWidget *parent) : QWidget(parent) {
        hbox = std::make_unique<QHBoxLayout>(this);
        newProjectButton = std::make_unique<QPushButton>(tr("New Project"));
        openProjectButton = std::make_unique<QPushButton>(tr("Open Project"));

        QObject::connect(newProjectButton.get(), &QPushButton::clicked, 
                this, &LauncherWidget::onNewProjectClicked);
        QObject::connect(openProjectButton.get(), &QPushButton::clicked, 
                this, &LauncherWidget::onOpenProjectClicked);
        
        hbox->addWidget(newProjectButton.get());
        hbox->addWidget(openProjectButton.get());
        setLayout(hbox.get());
        setFixedSize(192, 96);
    }

    void LauncherWidget::onNewProjectClicked(bool checked) {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::Directory);
        int result = dialog.exec();
        QString dirPath;
        if (result) {
            dirPath = dialog.selectedFiles()[0];
            QString name = dirPath.mid(dirPath.lastIndexOf("/") + 1);
            emit newProject(name, dirPath);
        }
    }

    void LauncherWidget::onOpenProjectClicked(bool checked) {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setNameFilter(tr("Mechanizm Project Directory"));
        int result = dialog.exec();
        QString dirPath;
        if (result) {
            dirPath = dialog.selectedFiles()[0];
            emit openProjectDirectory(dirPath);
        }
    }

}