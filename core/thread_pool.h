#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace Core
{
  struct Job
  {
    typedef std::shared_ptr<Job> Sptr;
    typedef std::function<void()> Routine;

    std::string Id;
    Routine Context;
  };

  class ThreadPool
  {
  public:
    typedef std::shared_ptr<ThreadPool> Sptr;
    typedef std::unique_ptr<ThreadPool> Uptr;

    static const std::uint32_t MinThreads = 1;
    static const std::uint32_t MaxThreads = 4;

    virtual ~ThreadPool() {}

    virtual void Shedule(Job::Sptr job) = 0;
    virtual void Shedule(const std::string& id, const Job::Routine& context) = 0;

    virtual void Submit() = 0;
    virtual void Cancel() = 0;
    virtual void Wait() = 0;
  };

  ThreadPool::Uptr CreateStandardThreadPool(std::uint32_t minThreads = ThreadPool::MinThreads, std::uint32_t maxThreads = ThreadPool::MaxThreads); // TODO: support min threads
  ThreadPool::Uptr CreateNativeThreadPool(std::uint32_t minThreads = ThreadPool::MinThreads, std::uint32_t maxThreads = ThreadPool::MaxThreads);
}
