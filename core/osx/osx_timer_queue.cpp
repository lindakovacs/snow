#include "osx_timer_queue.h"
#include <exception>

namespace Core
{
  dispatch_source_t CreateDispatchTimer(std::uint64_t dueTime, std::uint64_t period, dispatch_queue_t queue, dispatch_block_t block)
  {
    dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    if (timer)
    {
      dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, dueTime * NSEC_PER_MSEC), period * NSEC_PER_MSEC, 100ull * NSEC_PER_MSEC);
      dispatch_source_set_event_handler(timer, block);
      dispatch_resume(timer);
    }
    return timer;
  }

  MacTimer::MacTimer(TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period)
  : m_callback(callback)
  {
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    m_timer = CreateDispatchTimer(dueTime, period, queue, ^{ callback(); });
  }

  MacTimer::~MacTimer()
  {
    // NOTE: the function doesn't wait for any running timer callback functions to complete before returning
    if (m_timer)
    {
      dispatch_source_cancel(m_timer);
      dispatch_release(m_timer); // remove this if you are on a Deployment Target of iOS6 or OSX 10.8 and above
      m_timer = nil;
    }    
  }

  MacTimerQueue::MacTimerQueue()
  {
  }

  void MacTimerQueue::Reset(const std::string& timerId, TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period)
  {
    m_timers[timerId] = MacTimer::Sptr(new MacTimer(callback, dueTime, period));
  }

  void MacTimerQueue::Erase(const std::string& timerId)
  {
    m_timers.erase(timerId);
  }

  MacTimerQueue::~MacTimerQueue()
  {
    m_timers.clear();
  }

  TimerQueue::Sptr CreateTimerQueue()
  {
    return TimerQueue::Sptr(new MacTimerQueue());
  }
}
