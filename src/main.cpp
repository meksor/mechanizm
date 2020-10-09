#include "OpenShot.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
  int frame = 0;
  openshot::FFmpegReader r(argv[1]);
  r.Open(); // Open the reader
  // Get frame number 1 from the video
  std::shared_ptr<openshot::Frame> f;
  while (f = r.GetFrame(frame)) { 
    // Now that we have an openshot::Frame object, lets have some fun!
    f->Display(); // Display the frame on the screen
    // f->DisplayWaveform(); // Display the audio waveform as an image
    // f->Play(); // Play the audio through your speaker
    // Close the reader
    frame += 1;
  }
  r.Close();
  return 0;
}