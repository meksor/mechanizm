#include "compositor.h"
#include "Clip.h"
#include "mapping.h"
#include "sequence.h"

namespace mechanizm {
Compositor::Compositor(){};

void Compositor::compose(mechanizm::Mapping *m) {
  auto c = m->clip;
  openshot::Clip os_clip = openshot::Clip(c->source->reader);
  os_clip.Start(c->getFirstFrame());
  os_clip.End(c->getLastFrame());
  openshot::Fraction fps = os_clip.Reader()->info.fps;
  double fpm = (60. * fps.ToFloat());
  double quarterNoteLength = fpm / this->bpm;
  auto rythmicPoints = c->rythmicPoints;
  auto chTs = m->getChannelTimeseries();

  for (auto step : chTs) {
    const auto [timeStep, channel] = step;
  }
};

} // namespace mechanizm