

#include "clip.h"
#include "mapping.h"

namespace mechanizm {
class Compositor {
public:
  Compositor();

  void setBpm(float b) { bpm = b; };
  void compose(mechanizm::Mapping *m);

private:
  float bpm;
};

} // namespace mechanizm