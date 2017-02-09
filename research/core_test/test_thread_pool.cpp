#include "test_thread_pool.h"

#include <core/thread_pool.h>
#include <core/tracer.h>

#include <chrono>
//#include <functional>
#include <thread>

namespace Research
{
  void Routine(const std::wstring& name)
  {
    for (std::uint32_t index = 0; index < 10; ++index)
    {
      CoreTrace(TraceLevel::Information, name);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

  void TestThreadPool()
  {
    Core::ThreadPool::Uptr pool = Core::CreateNativeThreadPool();

    pool->Shedule(1, std::bind(Routine, L"routine-01"));
    pool->Shedule(2, std::bind(Routine, L"routine-02"));
    pool->Shedule(3, std::bind(Routine, L"routine-03"));
    pool->Shedule(4, std::bind(Routine, L"routine-04"));

    pool->Submit();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    pool->Cancel();
    pool->Wait();
  }
}
