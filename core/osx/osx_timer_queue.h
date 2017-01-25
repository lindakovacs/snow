#pragma once

#include <core/timer_queue.h>
#import <Foundation/Foundation.h>

namespace Core
{
  class MacTimer
  {
  public:
    typedef std::shared_ptr<MacTimer> Sptr;
    typedef std::map<std::string, Sptr> Map;

    MacTimer(TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period);
    virtual ~MacTimer();

  private:
    dispatch_source_t m_timer;
    TimerQueue::CallbackType m_callback;
  };

  class MacTimerQueue: public TimerQueue
  {
  public:
    MacTimerQueue();
    virtual ~MacTimerQueue();
    virtual void Reset(const std::string& timerId, TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period);
    virtual void Erase(const std::string& timerId);

  private:
    MacTimer::Map m_timers;
  };

  TimerQueue::Sptr CreateTimerQueue();
}
