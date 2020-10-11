#include "window.h"



MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    this->setWindowTitle("Mechanizm");
    this->setFixedSize(width, height);

    renderWidget = std::make_unique<VideoRenderWidget>(this);
    renderWidget->setFixedSize(width, height);

    player = std::make_unique<openshot::QtPlayer>(renderWidget->GetRenderer());
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_J:
        playerSpeed += .25;
        break;
    case Qt::Key_K:
        if (playerSpeed == 0.) {
            playerSpeed = 1.;
        } else {
            playerSpeed = 0.;
        }
        break;
    case Qt::Key_L:
        playerSpeed -= .25;
        break;
    default:
        break;
    }
    player->Speed(playerSpeed);
}


void MainWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    player->Play();
} 
