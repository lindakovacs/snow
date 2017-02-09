#pragma once

#include <memory>
#include <string>

namespace Core
{
  template <typename T>
  class QueueItemProcessor
  {
  public:
    typedef std::shared_ptr<QueueItemProcessor> Sptr;

    virtual ~QueueItemProcessor() {}
    virtual void Process(const T& queueItem, volatile bool& cancelFlag) = 0; // no throwing
  };
}
