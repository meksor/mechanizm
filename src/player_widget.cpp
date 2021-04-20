#include <OpenShot.h>

#include "player_widget.h"

namespace mechanizm {

    PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent) {
        renderWidget = std::make_unique<VideoRenderWidget>(this);
        player = std::make_shared<openshot::QtPlayer>(renderWidget->GetRenderer());
    }

    void PlayerWidget::keyPressEvent(QKeyEvent *event) {
        switch (event->key()) {
        case Qt::Key_L:
            event->accept();
            playerSpeed += 1.;
            player->Speed(playerSpeed);
            break;
        case Qt::Key_K:
            event->accept();
            if (playerSpeed == 0.) {
                playerSpeed = 1.;
            } else {
                playerSpeed = 0.;
            }
            player->Speed(playerSpeed);
            break;
        case Qt::Key_J:
            event->accept();
            playerSpeed -= 1.;
            player->Speed(playerSpeed);
            break;
        case Qt::Key_Space:
            event->accept();
            if(player->Mode() == openshot::PLAYBACK_PLAY) {
                player->Pause();
            } else {
                playerSpeed = 1.;
                player->Speed(playerSpeed);
                player->Play();
            }
            break;
        case Qt::Key_Left:
            event->accept();
            playerSpeed = 0.;
            player->Speed(playerSpeed);
            player->Seek(player->Position() - 1); 
            break;
        case Qt::Key_Right:
            event->accept();
            playerSpeed = 0.;
            player->Speed(playerSpeed);
            player->Seek(player->Position() + 1); 
            break;
        default:
            event->ignore();
            break;
        }
    }

    void PlayerWidget::showEvent(QShowEvent* event) {
        QWidget::showEvent(event);
        player->Seek(0);
        player->Play();
        event->accept();
    } 

    void PlayerWidget::closeEvent(QCloseEvent *event) {
        QWidget::closeEvent(event);
        player->Stop();
        event->accept();
    }

    void PlayerWidget::readFile(QString filePath) {
        setReader(new openshot::FFmpegReader(filePath.toStdString())); // TODO: no
    }

    void PlayerWidget::setReader(openshot::ReaderBase *r) {
        r->Open();
        player->Reader(r);
        player->Seek(0);

        this->setFixedSize(r->info.width, r->info.height);
        renderWidget->setFixedSize(r->info.width, r->info.height);
    }

}