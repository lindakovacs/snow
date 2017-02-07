#pragma once

#include <windows.h>

namespace Core
{
  class WinSpinlockMutex
  {
  public:
    WinSpinlockMutex()
    {
      ::InitializeCriticalSection(&CriticalSection);
    }

    ~WinSpinlockMutex()
    {
      ::DeleteCriticalSection(&CriticalSection);
    }

    void Lock()
    {
      ::EnterCriticalSection(&CriticalSection);
    }

    void Unlock()
    {
      ::LeaveCriticalSection(&CriticalSection);
    }

    WinSpinlockMutex(const WinSpinlockMutex&) = delete;
    WinSpinlockMutex& operator = (const WinSpinlockMutex&) = delete;

  private:
    CRITICAL_SECTION CriticalSection;
  };
}
