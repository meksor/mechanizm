#include <iostream>
#include <memory>

#include <Clip.h>
#include <QtPlayer.h>
#include <Qt/VideoRenderWidget.h>
#include <qapplication.h>
#include <QSizePolicy>
#include <QVBoxLayout>

#include "browser.h"
#include "editor.h"
#include "project.h"
#include "ui.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  auto project = std::make_shared<mechanizm::Project>();
  project->addFile(argv[1]);
  mechanizm::ProjectUiController uiController;
  uiController.setProject(project);
  uiController.openBrowserWindow();
  
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
