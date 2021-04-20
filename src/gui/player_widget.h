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

    class PlayerWidget : public QWidget {
        Q_OBJECT
    public:
        typedef std::unique_ptr<PlayerWidget> unique_ptr;

        explicit PlayerWidget(QWidget *parent = 0);
        virtual ~PlayerWidget() {};

        void readFile(QString filePath);
        void setReader(openshot::ReaderBase *r);
        std::shared_ptr<openshot::QtPlayer> getPlayer() {
            return player;
        }
        virtual void keyPressEvent(QKeyEvent *event) override;

    protected:
        void showEvent(QShowEvent* event) override;
        void closeEvent(QCloseEvent *event) override;
        std::shared_ptr<openshot::QtPlayer> player;
        std::unique_ptr<VideoRenderWidget> renderWidget;


    private:

        float playerSpeed = 1.0;
    };
    
}