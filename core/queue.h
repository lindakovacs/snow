#pragma once

#include <deque>
#include <memory>
#include <mutex>
#include <string>

namespace Core
{
  template <typename T>
  class Queue
  {
  public:
    typedef std::shared_ptr<Queue> Sptr;

    explicit Queue(const std::string& selfId)
    : SelfId(selfId)
    {
    }

    std::string GetId() const
    {
      return SelfId;
    }

    std::uint64_t GetSize() const
    {
      std::lock_guard<std::mutex> lock(Guard);
      return static_cast<std::uint64_t>(Entries.size());
    }

    bool Pop(T& entry)
    {
      std::lock_guard<std::mutex> lock(Guard);
      if (Entries.empty())
      {
        return false;
      }
      entry = std::move(Entries.front());
      Entries.pop_front();
      return true;
    }

    void Push(T entry)
    {
      std::lock_guard<std::mutex> lock(Guard);
      Entries.push_back(std::move(entry));
    }

  private:
    const std::string SelfId;
    std::deque<T> Entries;
    mutable std::mutex Guard;
  };
}
