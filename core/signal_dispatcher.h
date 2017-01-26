#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace Core
{
  namespace Signal
  {
    const std::uint32_t None = 0;
    const std::uint32_t Cancel = 1;
    const std::uint32_t Update = 2;
    const std::uint32_t Completed = 4;
  }

  // const std::string AnyEmitter = "any-emitter";  // TODO: ?

  class SignalDispatcher
  {
  public:
    typedef std::shared_ptr<SignalDispatcher> Sptr;
    typedef std::function<void(const std::string&, std::uint32_t)> Routine; // emitter id, signals

    SignalDispatcher();
    ~SignalDispatcher();

    void Emit(const std::string& emitterId, std::uint32_t  actualSignal);
    void Subscribe(const std::string& subscriberId, const std::string& emitterId, std::uint32_t expectedSignals, const Routine& callback);
    void Unsubscribe(const std::string& subscriberId);

  private:
    SignalDispatcher(const SignalDispatcher&);
    SignalDispatcher& operator = (const SignalDispatcher&);

  private:
    struct Subscriber
    {
      typedef std::shared_ptr<Subscriber> Ptr;
      typedef std::map<std::string, Ptr> Map;

      Subscriber(std::uint32_t expectedSignals, const Routine& callback);
      std::uint32_t ExpectedSignals;
      Routine Callback;
    };

    // <emitter id, <subscriber id, data>>
    typedef std::map<std::string, Subscriber::Map> Subscribers;
    Subscribers m_subscribers;
    std::mutex m_guard;
  };
}
