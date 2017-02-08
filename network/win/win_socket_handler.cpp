#include "win_socket_handler.h"
#include <core/string_utils.h>

namespace Network
{
  WinSocketHandler::WinSocketHandler(std::uint32_t threadsNumber)
    : PortHandle(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, threadsNumber))
  {
    if (!PortHandle)
    {
      throw std::runtime_error(Core::Format("can't create i/o completion port, error: %d\n", ::GetLastError()));
    }
  }

  WinSocketHandler::~WinSocketHandler()
  {
    ::CloseHandle(PortHandle);
  }

  void WinSocketHandler::Emit(std::uint64_t emitterId, std::uint32_t actualSignal)
  {
    PostQueuedCompletionStatus(g_hIOCP, 0, 0, NULL);
  }

  void WinSocketHandler::Subscribe(std::uint64_t subscriberId, std::uint64_t emitterId, std::uint32_t expectedSignals, const Routine& callback)
  {
    if (::CreateIoCompletionPort(static_cast<HANDLE>(subscriberId), PortHandle, static_cast<DWORD_PTR>(subscriberId), 0) == nullptr)
    {
      throw std::runtime_error(Core::Format("can't subscribe, subscriber: %#010x, emitter: %#010x, error: %d\n", subscriberId, emitterId, ::GetLastError()));
    }
  }

  void WinSocketHandler::Unsubscribe(std::uint64_t subscriberId)
  {
  }

  SignalDispatcher::Sptr CreateSignalDispatcher()
  {
    return SignalDispatcher::Sptr(new WinSocketHandler());
  }
}
