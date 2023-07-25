#pragma once

#include "Clip.h"
#include "clip.h"
#include "mapping.h"
#include <libopenshot/Timeline.h>

namespace mechanizm {
class Compositor {
public:
  Compositor();

  void setBpm(float b) { bpm = b; };
  openshot::Clip *compose(mechanizm::Mapping *m);

private:
  openshot::Clip *os_clip = nullptr;
  float bpm;
};

} // namespace mechanizm