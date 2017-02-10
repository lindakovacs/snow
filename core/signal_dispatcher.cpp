#include "signal_dispatcher.h"
#include <mutex>

namespace Core
{
  SignalDispatcher::Subscriber::Subscriber(std::uint32_t expectedSignals, const Routine& callback)
    : ExpectedSignals(expectedSignals)
    , Callback(callback)
  {
  }

  SignalDispatcher::SignalDispatcher()
  {
  }

  SignalDispatcher::~SignalDispatcher()
  {
  }

  void SignalDispatcher::Emit(std::uint64_t emitterId, std::uint32_t actualSignal)
  {
    std::lock_guard<decltype(Guard)> lock(Guard);

    Subscribers::const_iterator iter = Catalog.find(emitterId);
    if (iter == Catalog.end())
    {
      return;
    }

    for (const auto& item : iter->second)
    {
      const auto& subscriber = item.second;
      if (subscriber->ExpectedSignals & actualSignal)
      {
        (subscriber->Callback)(emitterId, actualSignal);
      }
    }
  }

  void SignalDispatcher::Subscribe(std::uint64_t subscriberId, std::uint64_t emitterId, std::uint32_t expectedSignals, const Routine& callback)
  {
    std::lock_guard<decltype(Guard)> lock(Guard);

    Subscribers::iterator iter = Catalog.find(emitterId);
    if (iter == Catalog.end())
    {
      Subscriber::Map subscribers;
      subscribers[subscriberId] = std::make_shared<Subscriber>(expectedSignals, callback);
      Catalog[emitterId] = subscribers;
    }
    else
    {
      iter->second[subscriberId] = std::make_shared<Subscriber>(expectedSignals, callback);
    }
  }

  void SignalDispatcher::Unsubscribe(std::uint64_t subscriberId)
  {
    std::lock_guard<decltype(Guard)> lock(Guard);

    for (Subscribers::iterator iter = Catalog.begin(); iter != Catalog.end();)
    {
      Subscriber::Map& items = iter->second;
      items.erase(subscriberId);
      if (items.empty())
      {
        Catalog.erase(iter++);
      }
      else
      {
        ++iter;
      }
    }
  }
}
