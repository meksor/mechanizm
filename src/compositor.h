#pragma once

#include "Clip.h"
#include "clip.h"
#include "mapping.h"
#include <libopenshot/Timeline.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace mechanizm {
class Compositor {
public:
  Compositor();

  void setBpm(float b) { bpm = b; };
  openshot::Clip *compose(mechanizm::Mapping *m);

private:
  openshot::Clip *os_clip = nullptr;
  std::unordered_map<std::string, std::unique_ptr<openshot::Clip>> clipCache;
  float bpm;
};

} // namespace mechanizm