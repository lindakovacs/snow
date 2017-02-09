#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

namespace Core
{
  namespace Signal
  {
    const std::uint32_t None = 0;
    const std::uint32_t Cancel = 1;
    const std::uint32_t Update = 2;
    const std::uint32_t Completed = 4;
  }

  class SignalDispatcher
  {
  public:
    typedef std::shared_ptr<SignalDispatcher> Sptr;
    typedef std::function<void(std::uint64_t, std::uint32_t)> Routine; // emitter id, signals

    SignalDispatcher();
    ~SignalDispatcher();

    void Emit(std::uint64_t emitterId, std::uint32_t actualSignal);
    void Subscribe(std::uint64_t subscriberId, std::uint64_t emitterId, std::uint32_t expectedSignals, const Routine& callback);
    void Unsubscribe(std::uint64_t subscriberId);

    SignalDispatcher(const SignalDispatcher&) = delete;
    SignalDispatcher& operator = (const SignalDispatcher&) = delete;

  private:
    struct Subscriber
    {
      typedef std::shared_ptr<Subscriber> Ptr;
      typedef std::unordered_map<std::uint64_t, Ptr> Map;

      Subscriber(std::uint32_t expectedSignals, const Routine& callback);
      std::uint32_t ExpectedSignals;
      Routine Callback;
    };

    // <emitter id, <subscriber id, data>>
    typedef std::unordered_map<std::uint64_t, Subscriber::Map> Subscribers;
    Subscribers Catalog;

    std::shared_mutex Guard;
  };
}
