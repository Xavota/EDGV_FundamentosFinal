#pragma once
#include "platform/defines.h"
#include "platform/module.hpp"

namespace gl {
  /**
   * @brief  Keeps track of the global time and can create, update and destroy timers 
   *         accesible in any part of the document
   */
  class Time : public Module<Time>
  {
   public:
    /**
     * @brief  Default constructor
     */
    Time() = default;
    /**
     * @brief  Default destructor
     */
    ~Time() = default;

    /**
     * @brief  Returns the time between the last update and this
     * @bug    No know Bugs
     */
    float deltaTime() const { return mTime; }

    /**
     * @brief  Adds a timer with a name
     * @param    name: the name to identify the timer
     * @bug    No know Bugs
     */
    void addTimer(const String& name);
    /**
     * @brief  Returns the value of a timer
     * @param    name: the name to identify the timer
     * @bug    No know Bugs
     */
    float getTimer(const String& name) const;
    /**
     * @brief  Sets the value of a timer
     * @param    name: the name to identify the timer
     * @param    time: the new time
     * @bug    No know Bugs
     */
    void setTimer(const String& name, const float time);
    /**
     * @brief  Set a timer to start adding time
     * @param    name: the name to identify the timer
     * @bug    No know Bugs
     */
    void startTimer(const String& name);
    /**
     * @brief  Stop a timer from addingtime
     * @param    name: the name to identify the timer
     * @bug    No know Bugs
     */
    void stopTimer(const String& name);
    /**
     * @brief  Returns a timer's value to 0
     * @param    name: the name to identify the timer
     * @bug    No know Bugs
     */
    void restartTimer(const String& name);
    /**
     * @brief  Deletes a timer
     * @param    name: the name to identify the timer
     * @bug    No know Bugs
     */
    void deleteTimer(const String& name);
    /**
     * @brief  Returns true if the timers asked already exist
     * @param    name: the name to identify the timer
     * @bug    No know Bugs
     */
    bool timerExist(const String& name) const;

    /**
     * @brief  Updates the world time along with all the timers
     * @param    time: time passed since the last update
     * @bug    No know Bugs
     */
    void update(const float time);

   private:
    friend class FrameworkManager;
    /**
     * @brief  Data for the stores timers
     */
    struct Timer
    {
      /**
       * @brief  The current time of the timer
       */
      float time;
      /**
       * @brief  If the timer is adding time or not
       */
      bool playing;
    };

    /**
     * @brief  The current time
     */
    float mTime;
    /**
     * @brief  The array of timers and their states
     */
    Map<String, Timer> mTimers;
  };
}
