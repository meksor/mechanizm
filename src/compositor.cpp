#include "compositor.h"
#include "Clip.h"
#include "Point.h"
#include "clip.h"
#include "mapping.h"
#include "sequence.h"

namespace mechanizm {

static int wrapLoop(int point, int size) {
  if (size <= 0) return 0;
  return ((point % size) + size) % size;
}

static int wrapBounce(int point, int size) {
  if (size <= 1) return 0;
  int period = 2 * (size - 1);
  int mod = ((point % period) + period) % period;
  return (mod < size) ? mod : period - mod;
}

Compositor::Compositor(){};

openshot::Clip *Compositor::compose(mechanizm::Mapping *m) {
  if (m == nullptr || m->clip == nullptr || m->clip->source == nullptr) {
    return nullptr;
  }

  auto c = m->clip;
  if (os_clip != nullptr) {
    delete os_clip;
  }
  os_clip = new openshot::Clip(c->source->path);
  if (!os_clip->Reader()->IsOpen()) {
    os_clip->Reader()->Open();
  }

  openshot::Fraction fps = os_clip->Reader()->info.fps;
  // os_clip->Start(c->getFirstFrame() * fps.ToFloat());
  // double end = c->getLastFrame() / fps.ToFloat();
  // os_clip->End(end);
  double fpm = (60. * fps.ToFloat());
  double nl = fpm / this->bpm;
  auto rythmicPoints = c->rythmicPoints;
  if (rythmicPoints.empty()) {
    return os_clip;
  }

  auto chTs = m->getChannelTimeseries();

  int point = 0;
  double maxT = 0;
  for (auto step : chTs) {
    const auto [timeStep, channel] = step;
    if (channel == nullptr) {
      continue;
    }

    if (channel->effect == mechanizm::Channel::Effect::INC) {
      point = point + channel->value;
    } else if (channel->effect == mechanizm::Channel::Effect::DEC) {
      point = point - channel->value;
    } else if (channel->effect == mechanizm::Channel::Effect::SET) {
      point = channel->value;
    }

    if (m->wrapBehaviour == mechanizm::Mapping::WrapBehaviour::BOUNCE) {
      point = wrapBounce(point, static_cast<int>(rythmicPoints.size()));
    } else {
      point = wrapLoop(point, static_cast<int>(rythmicPoints.size()));
    }

    maxT = round(nl * timeStep.note);
    openshot::Point timePoint(maxT, rythmicPoints[point].frame,
                              openshot::InterpolationType::BEZIER);
    os_clip->time.AddPoint(timePoint);
  }
  if (os_clip->time.GetCount() > 1) {
    os_clip->time.UpdatePoint(0, os_clip->time.GetPoint(1));
  }
  return os_clip;
};

} // namespace mechanizm