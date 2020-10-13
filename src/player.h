#pragma once 

#include <memory>
#include <ReaderBase.h>
#include <QtPlayer.h>
#include <Qt/VideoRenderWidget.h>

#include <QWidget>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>


namespace mechanizm {
    class PlayerWindow : public QWidget {
        Q_OBJECT
    public:
        typedef std::shared_ptr<PlayerWindow> shared_ptr;

        explicit PlayerWindow(QWidget *parent = 0);
        virtual ~PlayerWindow() {};

        void setReader(openshot::ReaderBase *r);

    protected:
        virtual void keyPressEvent(QKeyEvent *event) override;
        void showEvent(QShowEvent* event) override;
        void closeEvent(QCloseEvent *event) override;

        std::shared_ptr<openshot::QtPlayer> player;

    private:
        std::unique_ptr<VideoRenderWidget> renderWidget;

        float playerSpeed = 1.0;
    };
}