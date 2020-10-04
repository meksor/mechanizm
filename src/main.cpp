#include "OpenShot.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
  FFmpegReader r(argv[1]);
  r.Open(); // Open the reader
  // Get frame number 1 from the video
  std::shared_ptr<Frame> f = r.GetFrame(1);
  // Now that we have an openshot::Frame object, lets have some fun!
  f->Display(); // Display the frame on the screen
  f->DisplayWaveform(); // Display the audio waveform as an image
  f->Play(); // Play the audio through your speaker
  // Close the reader
  r.Close();
  return 0;
}