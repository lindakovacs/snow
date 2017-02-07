#pragma once

#include <atomic>

namespace Core
{
  class LinSpinlockMutex
  {
  public:
    LinSpinlockMutex()
      : Flag(ATOMIC_FLAG_INIT)
    {
    }

    ~LinSpinlockMutex()
    {
    }

    void Lock()
    {
      while (Flag.test_and_set(std::memory_order_acquire));
    }

    void Unlock()
    {
      Flag.clear(std::memory_order_release);
    }

    LinSpinlockMutex(const LinSpinlockMutex&) = delete;
    LinSpinlockMutex& operator = (const LinSpinlockMutex&) = delete;

  private:
    std::atomic_flag Flag;TraceLock = ATOMIC_FLAG_INIT;
  };
}
