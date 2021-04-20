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
        
    protected:
        void showEvent(QShowEvent* event) override;
        void closeEvent(QCloseEvent *event) override;
        virtual void keyPressEvent(QKeyEvent *event) override;
        std::shared_ptr<openshot::QtPlayer> player;


    private:
        std::unique_ptr<VideoRenderWidget> renderWidget;

        float playerSpeed = 1.0;
    };
    
}