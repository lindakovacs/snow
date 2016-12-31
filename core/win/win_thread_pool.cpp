#include "win_thread_pool.h"

namespace Core
{
  VOID CALLBACK ThreadRoutine(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work)
  {
    Job::Routine* routine = static_cast<Job::Routine*>(context);
    if (!routine)
    {
      throw 1;
    }
    (*routine)();
  }

  PoolEnvironment::PoolEnvironment()
  {
    ::InitializeThreadpoolEnvironment(&Environment);
  }

  PoolEnvironment::~PoolEnvironment()
  {
    ::DestroyThreadpoolEnvironment(&Environment);  
  }

  PTP_CALLBACK_ENVIRON PoolEnvironment::Get()
  {
    return &Environment;
  }

  RoutineHolder::RoutineHolder(const Job::Routine& context, PTP_CALLBACK_ENVIRON environment)
  : Context(context)
  {
    Work = ::CreateThreadpoolWork(ThreadRoutine, &Context, environment);
    if (!Work)
    {
      throw 1;
    }
  }

  RoutineHolder::~RoutineHolder()
  {
    ::CloseThreadpoolWork(Work);
  }

  void RoutineHolder::Submit()
  {
    ::SubmitThreadpoolWork(Work);
  }

  void RoutineHolder::Wait()
  {
    ::WaitForThreadpoolWorkCallbacks(Work, true);
  }

  WinThreadPool::WinThreadPool(std::uint32_t minThreads, std::uint32_t maxThreads)
  {
    Pool = ::CreateThreadpool(NULL);
    if (!Pool)
    {
      throw ::GetLastError(); // TODO: make Core::Error
    }
    if (!::SetThreadpoolThreadMinimum(Pool, minThreads))
    {
      throw ::GetLastError(); // TODO: make Core::Error
    }
    ::SetThreadpoolThreadMaximum(Pool, maxThreads);
    ::SetThreadpoolCallbackPool(Environment.Get(), Pool);
  }

  WinThreadPool::~WinThreadPool()
  {
    ::CloseThreadpool(Pool);
  }

  void WinThreadPool::Shedule(const std::string& id, const Job::Routine& context)
  {
    Contract[id] = RoutineHolder::Ptr(new RoutineHolder(context, Environment.Get()));
  }

  void WinThreadPool::Submit()
  {
    for (RoutineHolder::Map::const_iterator iter = Contract.begin(); iter != Contract.end(); ++iter)
    {
      iter->second->Submit();
    }
  }

  void WinThreadPool::Wait()
  {
    for (RoutineHolder::Map::const_iterator iter = Contract.begin(); iter != Contract.end(); ++iter)
    {
      iter->second->Wait();
    }
  }

  ThreadPool::Uptr CreateNativeThreadPool(std::uint32_t minThreads, std::uint32_t maxThreads)
  {
    return ThreadPool::Uptr(new WinThreadPool(minThreads, maxThreads));
  }
}
