#include "signal_dispatcher.h"

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

  void SignalDispatcher::Emit(const std::string& emitterId, std::uint32_t actualSignal)
  {
    std::lock_guard<std::mutex> lock(m_guard);

    Subscribers::const_iterator iter = m_subscribers.find(emitterId);
    if (iter == m_subscribers.end())
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

  void SignalDispatcher::Subscribe(const std::string& subscriberId, const std::string& emitterId, std::uint32_t expectedSignals, const Routine& callback)
  {
    std::lock_guard<std::mutex> lock(m_guard);

    Subscribers::iterator iter = m_subscribers.find(emitterId);
    if (iter == m_subscribers.end())
    {
      Subscriber::Map subscribers;
      subscribers[subscriberId] = std::make_shared<Subscriber>(expectedSignals, callback);
      m_subscribers[emitterId] = subscribers;
    }
    else
    {
      iter->second[subscriberId] = std::make_shared<Subscriber>(expectedSignals, callback);
    }
  }

  void SignalDispatcher::Unsubscribe(const std::string& subscriberId)
  {
    std::lock_guard<std::mutex> lock(m_guard);

    for (Subscribers::iterator iter = m_subscribers.begin(); iter != m_subscribers.end();)
    {
      Subscriber::Map& items = iter->second;
      items.erase(subscriberId);
      if (items.empty())
      {
        m_subscribers.erase(iter++);
      }
      else
      {
        ++iter;
      }
    }
  }
}
