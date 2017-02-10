#include <core/thread_pool.h>

namespace Core
{
  ThreadPool::Uptr CreateNativeThreadPool(std::uint32_t minThreads, std::uint32_t maxThreads)
  {
    // TODO: implementation real native tp
    return CreateStandardThreadPool(minThreads, maxThreads);
  }
}
