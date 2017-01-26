#include "osx_thread_pool.h"

namespace Core
{
  ThreadPool::Uptr CreateNativeThreadPool(std::uint32_t minThreads, std::uint32_t maxThreads)
  {
    return CreateStandardThreadPool(minThreads, maxThreads); // TODO: implement native threadpool
  }
}
