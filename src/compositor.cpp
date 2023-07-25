#include "compositor.h"
#include "Clip.h"
#include "Point.h"
#include "clip.h"
#include "mapping.h"
#include "sequence.h"

namespace mechanizm {
Compositor::Compositor(){};

openshot::Clip *Compositor::compose(mechanizm::Mapping *m) {
  auto c = m->clip;
  if (os_clip != nullptr) {
    delete os_clip;
  }
  os_clip = new openshot::Clip(c->source->path);
  openshot::Fraction fps = os_clip->Reader()->info.fps;
  // os_clip->Start(c->getFirstFrame() * fps.ToFloat());
  // double end = c->getLastFrame() / fps.ToFloat();
  // os_clip->End(end);
  double fpm = (60. * fps.ToFloat());
  double nl = fpm / this->bpm;
  auto rythmicPoints = c->rythmicPoints;
  auto chTs = m->getChannelTimeseries();

  int point = 0;
  double maxT = 0;
  for (auto step : chTs) {
    const auto [timeStep, channel] = step;
    if (channel->effect == mechanizm::Channel::Effect::INC) {
      point = point + channel->value;
    } else if (channel->effect == mechanizm::Channel::Effect::DEC) {
      point = point - channel->value;
    }
    if (point == rythmicPoints.size()) {
      point = 0;
    } else if (point == -1) {
      point = rythmicPoints.size() - 1;
    }

    maxT = round(nl * timeStep.note);
    openshot::Point timePoint(maxT, rythmicPoints[point].frame,
                              openshot::InterpolationType::BEZIER);
    os_clip->time.AddPoint(timePoint);
  }
  os_clip->time.UpdatePoint(0, os_clip->time.GetPoint(1));
  return os_clip;
};

} // namespace mechanizm