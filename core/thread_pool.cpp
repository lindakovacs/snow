#include "thread_pool.h"
#include "queue_processor.h"

#include <functional>
#include <map>
#include <sstream>
#include <thread>

namespace Core
{
  class JobProcessor: public QueueItemProcessor<Job::Sptr>
  {
  public:
    explicit JobProcessor(const std::string& selfId)
    {
    }

    virtual ~JobProcessor()
    {
    }

    virtual std::string GetId() const
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
    const std::string SelfId;
  };

  const std::string StandardThreadPoolQueueId = "standard-thread-pool-queue-id";
  const std::string StandardThreadPoolId = "standard-thread-pool-id";

  class StandardThreadPool: public ThreadPool
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

    virtual void Shedule(const std::string& id, const Job::Routine& context)
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
        const std::string suffix = std::to_string(index);
        const std::string itemProcessorId = "item-processor-id-" + suffix;
        const std::string queueProcessorId = "queue-processor-id-" + suffix;

        QueueItemProcessor<Job::Sptr>::Sptr itemProcessor(new JobProcessor(itemProcessorId));

        Thread::Sptr thr(new Thread());
        thr->Processor.reset(new QueueProcessor<Job::Sptr>(queueProcessorId, StandardThreadPoolId, JobQueue, itemProcessor, Dispatcher));
        thr->Engine.reset(new std::thread(std::bind(&QueueProcessor<Job::Sptr>::Execute, thr->Processor)));
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
      typedef std::map<std::string, Sptr> Map;

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
