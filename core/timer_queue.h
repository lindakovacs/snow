#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>

namespace Core
{
  class TimerQueue
  {
  public:
    typedef std::shared_ptr<TimerQueue> Sptr;
    typedef std::function<void()> CallbackType;

    virtual ~TimerQueue() {}
    virtual void Reset(const std::string& timerId, TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period = 0) = 0;
    virtual void Erase(const std::string& timerId) = 0;
  };

  TimerQueue::Sptr CreateTimerQueue();
}
