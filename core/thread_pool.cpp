#include "thread_pool.h"
#include "queue_processor.h"

#include <functional>
#include <map>
#include <thread>

namespace Core
{
  class JobProcessor : public QueueItemProcessor<Job::Sptr>
  {
  public:
    explicit JobProcessor(std::uint64_t selfId)
      : SelfId(selfId)
    {
    }

    virtual ~JobProcessor()
    {
    }

    virtual std::uint64_t GetId() const
    {
      return SelfId;
    }

    virtual void Process(const Job::Sptr& queueItem, volatile bool& cancelFlag)
    {
      try
      {
        queueItem->Context();
      }
      catch (...)
      {
      	// TODO: trace
      }
    }

  private:
    const std::uint64_t SelfId;
  };

  const std::uint64_t StandardThreadPoolQueueId = 10;
  const std::uint64_t StandardThreadPoolId = 20;

  class StandardThreadPool : public ThreadPool
  {
  public:
    StandardThreadPool(std::uint32_t minThreads, std::uint32_t maxThreads)
      : MinThreads(minThreads)
      , MaxThreads(maxThreads)
      , Dispatcher(new SignalDispatcher())
      , JobQueue(new Queue<Job::Sptr>(StandardThreadPoolQueueId))
    {
    }

    virtual ~StandardThreadPool()
    {
      Cancel();
      Wait();
    }

    virtual void Shedule(Job::Sptr job)
    {
      JobQueue->Push(job);

      Dispatcher->Emit(StandardThreadPoolId, Signal::Update);
    }

    virtual void Shedule(std::uint64_t id, const Job::Routine& context)
    {
      Job::Sptr job(new Job);
      job->Id = id;
      job->Context = context;
      JobQueue->Push(job);

      Dispatcher->Emit(StandardThreadPoolId, Signal::Update);
    }

    virtual void Submit()
    {
      for (std::uint32_t index = 0; index < MaxThreads; ++index)
      {
        const std::uint64_t itemProcessorId = index;
        const std::uint64_t queueProcessorId = index;

        QueueItemProcessor<Job::Sptr>::Sptr itemProcessor(new JobProcessor(index));

        Thread::Sptr thr = std::make_shared<Thread>();
        thr->Processor = std::make_shared<QueueProcessor<Job::Sptr>>(queueProcessorId, StandardThreadPoolId, JobQueue, itemProcessor, Dispatcher);
        thr->Engine = std::make_shared<std::thread>(std::bind(&QueueProcessor<Job::Sptr>::Execute, thr->Processor));
        ActiveJobs[queueProcessorId] = thr; // TODO: check call second Submit();

        std::this_thread::sleep_for(std::chrono::milliseconds(20)); // NOTE: switch to other thread, requered for mac
      }

      Dispatcher->Emit(StandardThreadPoolId, Signal::Update);
    }

    virtual void Cancel()
    {
      Dispatcher->Emit(StandardThreadPoolId, Signal::Cancel);
    }

    virtual void Wait()
    {
      for (Thread::Map::const_iterator iter = ActiveJobs.begin(); iter != ActiveJobs.end(); ++iter)
      {
        Thread::Sptr thr = iter->second;
        if (thr->Engine->joinable())
        {
          thr->Engine->join();
        }
      }
    }

  private:
    const std::uint32_t MinThreads; // TODO: support
    const std::uint32_t MaxThreads;

    SignalDispatcher::Sptr Dispatcher;
    Queue<Job::Sptr>::Sptr JobQueue;

    struct Thread
    {
      typedef std::shared_ptr<Thread> Sptr;
      typedef std::map<std::uint64_t, Sptr> Map;

      Thread() : Processor(), Engine() {}
      QueueProcessor<Job::Sptr>::Sptr Processor;
      std::shared_ptr<std::thread> Engine;
    };
    Thread::Map ActiveJobs;
  };

  ThreadPool::Uptr CreateStandardThreadPool(std::uint32_t minThreads, std::uint32_t maxThreads)
  {
    return ThreadPool::Uptr(new StandardThreadPool(minThreads, maxThreads));
  }
}
