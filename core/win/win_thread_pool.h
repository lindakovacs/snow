#pragma once

#include <libs/core/thread_pool.h>
#include <windows.h>
#include <map>

namespace Core
{
  class PoolEnvironment
  {
  public:
    PoolEnvironment();
    ~PoolEnvironment();
    PTP_CALLBACK_ENVIRON Get();

  private:
    TP_CALLBACK_ENVIRON Environment;
  };

  class RoutineHolder
  {
  public:
    typedef std::shared_ptr<RoutineHolder> Ptr;
    typedef std::map<std::string, Ptr> Map;

    RoutineHolder(const Job::Routine& context, PTP_CALLBACK_ENVIRON environment);
    ~RoutineHolder();
    void Submit();
    void Wait();

  private:
    Job::Routine Context;
    PTP_WORK Work;
  };

  class WinThreadPool: public ThreadPool
  {
  public:
    explicit WinThreadPool(std::uint32_t minThreads, std::uint32_t maxThreads);
    virtual ~WinThreadPool();

    virtual void Shedule(Job::Sptr job) {} // TODO: implement
    virtual void Shedule(const std::string& id, const Job::Routine& context);

    virtual void Submit();
    virtual void Cancel() {} // TODO: implement
    virtual void Wait();

  private:
    PTP_POOL Pool;
    PoolEnvironment Environment;
    RoutineHolder::Map Contract;
  };
}
