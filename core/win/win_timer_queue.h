#pragma once

#include <core/timer_queue.h>
#include <windows.h>

namespace Core
{
  class WinTimer
  {
  public:
    typedef std::shared_ptr<WinTimer> Sptr;
    typedef std::map<std::string, Sptr> Map;

    WinTimer(HANDLE timerQueueHandle, TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period);
    virtual ~WinTimer();

  private:
    const HANDLE m_timerQueueHandle;
    HANDLE m_timerHandle;
    TimerQueue::CallbackType m_callback;
  };

  class WinTimerQueue: public TimerQueue
  {
  public:
    WinTimerQueue();
    virtual ~WinTimerQueue();
    virtual void Reset(const std::string& timerId, TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period = 0);
    virtual void Erase(const std::string& timerId);

  private:
    HANDLE Handle;
    WinTimer::Map Timers;
  };
}
