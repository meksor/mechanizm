#pragma once

#include <memory>

#include <OpenShot.h>
#include <QtPlayer.h>
#include <Qt/VideoRenderWidget.h>

#include <QWidget>
#include <QKeyEvent>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() {};

    openshot::QtPlayer * Player() { return player.get(); };

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent* event) override;

private:
    std::unique_ptr<VideoRenderWidget> renderWidget;
    std::unique_ptr<openshot::QtPlayer> player;

    float playerSpeed = 1.0;
    int width = 1080;
    int height = 720;
};

