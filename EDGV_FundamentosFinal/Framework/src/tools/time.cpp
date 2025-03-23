#include "tools/time.h"

namespace gl {
  void Time::addTimer(const String& name)
  {
    mTimers[name] = Timer{ 0, true };
  }
  float Time::getTimer(const String& name) const
  {
    const auto it = mTimers.find(name);
    if (it == mTimers.end()) return 0.0f;

    return it->second.time;
  }
  void Time::setTimer(const String& name, const float time)
  {
    const auto it = mTimers.find(name);
    if (it == mTimers.end()) return;

    it->second.time = time;
  }
  void Time::startTimer(const String& name)
  {
    const auto it = mTimers.find(name);
    if (it == mTimers.end()) return;

    it->second.playing = true;
  }
  void Time::stopTimer(const String& name)
  {
    const auto it = mTimers.find(name);
    if (it == mTimers.end()) return;

    it->second.playing = false;
  }
  void Time::restartTimer(const String& name)
  {
    const auto it = mTimers.find(name);
    if (it == mTimers.end()) return;

    it->second.time = 0.0f;
  }
  void Time::deleteTimer(const String& name)
  {
    mTimers.erase(name);
  }
  bool Time::timerExist(const String& name) const
  {
    const auto it = mTimers.find(name);
    return it != mTimers.end();
  }
  void Time::update(const float time)
  {
    mTime = time;
    for (Map<String, Timer>::iterator it = mTimers.begin(); it != mTimers.end(); ++it)
    {
      if (it->second.playing)
      {
        it->second.time += time;
      }
    }
  }
}