#pragma once

#include "queue.h"
#include "queue_item_processor.h"
#include "signal_dispatcher.h"

#include <condition_variable>

namespace Core
{
  template <typename T>
  class QueueProcessor
  {
  public:
    typedef std::shared_ptr<QueueProcessor> Sptr;

    QueueProcessor(std::uint64_t selfId, std::uint64_t emitterId, typename Queue<T>::Sptr itemsQueue, typename QueueItemProcessor<T>::Sptr queueItemProcessor, SignalDispatcher::Sptr dispatcher)
      : SelfId(selfId)
      , EmitterId(emitterId)
      , ItemsQueue(itemsQueue)
      , ItemProcessor(queueItemProcessor)
      , Dispatcher(dispatcher)
      , CancelFlag(false)
      , UpdateFlag(false)
    {
    }

    ~QueueProcessor()
    {
    }

    std::uint64_t GetId() const
    {
      return SelfId;
    }

    void Execute()
    {
      Dispatcher->Subscribe(SelfId, EmitterId, Signal::Cancel | Signal::Update, std::bind(&QueueProcessor::OnSignal, this, std::placeholders::_1, std::placeholders::_2));
      while (true)
      {
        std::unique_lock<std::mutex> lock(Guard);

        while (!CancelFlag && !UpdateFlag)
        {
          NextProcessing.wait(lock);
        }
        UpdateFlag = false;
        if (CancelFlag)
        {
          break;
        }
        ProcessQueue();
      }
      Dispatcher->Unsubscribe(SelfId);
    }

  private:
    void OnSignal(std::uint64_t emitterId, std::uint32_t signals)
    {
      if (EmitterId != emitterId)
      {
        return;
      }
      if (signals & Signal::Cancel)
      {
        CancelFlag = true;
      }
      if (signals & Signal::Update)
      {
        UpdateFlag = true;
      }
      NextProcessing.notify_one();
    }

    void ProcessQueue()
    {
      T queueItem;
      while (!CancelFlag && ItemsQueue->Pop(queueItem))
      {
        ItemProcessor->Process(queueItem, CancelFlag);
      }
    }

  private:
    const std::uint64_t SelfId;
    const std::uint64_t EmitterId;

    typename Queue<T>::Sptr ItemsQueue;
    typename QueueItemProcessor<T>::Sptr ItemProcessor;
    SignalDispatcher::Sptr Dispatcher;

    volatile bool CancelFlag;
    volatile bool UpdateFlag;

    std::condition_variable NextProcessing;
    std::mutex Guard;
  };
}
