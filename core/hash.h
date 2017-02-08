#pragma once

#include <unordered_map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <string>

namespace Core
{
  template <typename KeyType, typename ValueType>
  class Hash
  {
  public:
    typedef std::shared_ptr<Hash> Sptr;

    Hash(const std::string& selfId, std::uint32_t capacity)
    : SelfId(selfId)
    {
      Entries.reserve(capacity);
    }

    std::string GetId() const
    {
      return SelfId;
    }

    std::uint64_t GetSize() const
    {
      std::shared_lock<std::shared_mutex> lock(Guard);
      return static_cast<std::uint64_t>(Entries.size());
    }

    ValueType Get(const KeyType& key)
    {
      std::shared_lock<std::shared_mutex> lock(Guard);
      std::unordered_map<KeyType, ValueType>::const_iterator iter = Entries.find(key);
      if (iter == Entries.end())
      {
        throw std::runtime_error("can't get a value from hash");
      }
      return *iter;
    }

    void Set(const KeyType& key, const ValueType& value)
    {
      std::unique_lock<std::shared_mutex> lock(Guard); 
      if (!Entries.emplace(key, value).second)
      {
        throw std::runtime_error("can't emplace a value to hash");
      }
    }

    void Remove(const KeyType& key)
    {
      std::unique_lock<std::shared_mutex> lock(Guard);
      Entries.erase(key);
    }

  private:
    const std::string SelfId;
    std::unordered_map<KeyType, ValueType> Entries;
    mutable std::shared_mutex Guard;
  };
}
