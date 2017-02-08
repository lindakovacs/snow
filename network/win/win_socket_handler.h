#pragma once

#include <network/socket_handler.h>
#include <windows.h>

namespace Network
{
  class WinSocketHandler : public SignalDispatcher
  {
  public:
    explicit WinSocketHandler(std::uint32_t threadsNumber);
    virtual ~WinSocketHandler();

    virtual void Emit(std::uint64_t emitterId, std::uint32_t actualSignal);
    virtual void Subscribe(std::uint64_t subscriberId, std::uint64_t emitterId, std::uint32_t expectedSignals, const Routine& callback);
    virtual void Unsubscribe(std::uint64_t subscriberId);

  private:
    HANDLE PortHandle;
  };
}
