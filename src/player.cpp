#include "player.h"
#include <OpenShot.h>

namespace mechanizm {

    PlayerWindow::PlayerWindow(QWidget *parent) : QWidget(parent) {
        renderWidget = std::make_unique<VideoRenderWidget>(this);
        player = std::make_shared<openshot::QtPlayer>(renderWidget->GetRenderer());
    }

    void PlayerWindow::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
        case Qt::Key_L:
            playerSpeed += 1.;
            player->Speed(playerSpeed);
            break;
        case Qt::Key_K:
            if (playerSpeed == 0.) {
                playerSpeed = 1.;
            } else {
                playerSpeed = 0.;
            }
            player->Speed(playerSpeed);
            break;
        case Qt::Key_J:
            playerSpeed -= 1.;
            player->Speed(playerSpeed);
            break;
        case Qt::Key_Space:
            if(player->Mode() == openshot::PLAYBACK_PLAY) {
                player->Pause();
            } else {
                player->Play();
            }
            break;
        default:
            break;
        }
    }

    void PlayerWindow::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);
        player->Play();
        event->accept();
    } 

    void PlayerWindow::closeEvent(QCloseEvent *event) {
        QWidget::closeEvent(event);
        player->Stop();
        event->accept();
    }

    void PlayerWindow::setReader(openshot::ReaderBase *r) {
        r->Open();
        player->Reader(r);

        this->setFixedSize(r->info.width, r->info.height);
        renderWidget->setFixedSize(r->info.width, r->info.height);
    }

}