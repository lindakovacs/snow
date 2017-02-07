#pragma once

#ifdef WIN32
#include <core/win/win_spinlock_mutex.h>
namespace Core
{
  typedef WinSpinlockMutex SpinlockMutex;
}
#elif
#include <core/lin/lin_spinlock_mutex.h>
namespace Core
{
  typedef LinSpinlockMutex SpinlockMutex;
}
#endif
