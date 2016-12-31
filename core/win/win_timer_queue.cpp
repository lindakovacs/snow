
#include "win_timer_queue.h"
#include <exception>

namespace Core
{
  VOID __stdcall Routine(PVOID parameter, BOOLEAN timerOrWaitFired)
  {
    try
    {
      TimerQueue::CallbackType* callback = reinterpret_cast<TimerQueue::CallbackType*>(parameter);
      (*callback)();
    }
    catch (const std::exception& error)
    {
      //Trace(error.what());
    }
    catch (...)
    {
      //Trace("Unknown error");
    }
  }

  WinTimer::WinTimer(HANDLE timerQueueHandle, TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period)
  : m_timerQueueHandle(timerQueueHandle)
  , m_callback(callback)
  {
    const ULONG flags = WT_EXECUTEDEFAULT | (period == 0 ? WT_EXECUTEONLYONCE : 0);
    if (!::CreateTimerQueueTimer(&m_timerHandle, m_timerQueueHandle, Routine, &m_callback, dueTime, period, flags))
    {
      throw ::GetLastError(); // TODO: make real error
    }
  }

  WinTimer::~WinTimer()
  {
    const std::uint32_t maxCounter = 4;
    std::uint32_t counter = 0;
    while (maxCounter > counter++)
    {

      // NOTE: If this parameter is INVALID_HANDLE_VALUE, the function waits for any running timer callback functions to complete before returning.
      // If this parameter is NULL, the function marks the timer for deletion and returns immediately.
      // If the timer has already expired, the timer callback function will run to completion. However, there is no notification sent when the timer callback function has completed.
      // Most callers should not use this option, and should wait for running timer callback functions to complete so they can perform any needed cleanup.

      if (::DeleteTimerQueueTimer(m_timerQueueHandle, m_timerHandle, NULL)) // TODO: INVALID_HANDLE_VALUE?
      {
        return;
      }
      const std::uint32_t error = ::GetLastError();
      // NOTE: if the error code is ERROR_IO_PENDING, it is not necessary to call this function again, for any other error, you should retry the call
      if (error == ERROR_IO_PENDING)
      {
        break;
      }
    }
  }

  WinTimerQueue::WinTimerQueue()
  : Handle(::CreateTimerQueue())
  {
    if (Handle == NULL)
    {
      throw ::GetLastError(); // TODO: make real error
    }
  }

  void WinTimerQueue::Reset(const std::string& timerId, TimerQueue::CallbackType callback, std::uint32_t dueTime, std::uint32_t period)
  {
    Timers[timerId] = WinTimer::Sptr(new WinTimer(Handle, callback, dueTime, period));
  }

  void WinTimerQueue::Erase(const std::string& timerId)
  {
    Timers.erase(timerId);
  }

  WinTimerQueue::~WinTimerQueue()
  {
    Timers.clear();
    if (!::DeleteTimerQueue(Handle))
    {
      //Trace(L"DeleteTimerQueue error");
    }
  }

  TimerQueue::Sptr CreateTimerQueue()
  {
    return TimerQueue::Sptr(new WinTimerQueue());
  }
}
