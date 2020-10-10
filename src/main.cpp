#include <iostream>
#include <memory>

#include <OpenShot.h>
#include <QtPlayer.h>
#include <Qt/VideoRenderWidget.h>
#include <qapplication.h>
#include <qpushbutton.h>
#include <QSizePolicy>
#include <QVBoxLayout>

#include "window.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  openshot::FFmpegReader reader(argv[1]);

  reader.Open();

  MainWindow window;
  window.Player()->Reader(&reader);
  window.show();

  return app.exec();
  /*openshot::Timeline t(1280, // width
           720, // height
           openshot::Fraction(25,1), // framerate
           44100, // sample rate
           2, // channels
           openshot::ChannelLayout::LAYOUT_STEREO
           );

  c.Start(0.0);
  c.End(16.0);
  t.AddClip(&c);

  player.Play();
  return 0;*/
}
