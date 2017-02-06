#include "win_resource_initialization.h"

#include <stdexcept>
#include <string>

namespace Network
{
  WinResourceInitialization::WinResourceInitialization()
  {
    if (::WSAStartup(MAKEWORD(2, 2), &WsaData))
    {
      throw std::runtime_error("WSAStartup failed with error: " + std::to_string(::WSAGetLastError()));
    }
  }

  WinResourceInitialization::~WinResourceInitialization()
  {
    ::WSACleanup();
  }

  ResourceInitialization::Sptr CreateResourceInitialization()
  {
    return ResourceInitialization::Sptr(new WinResourceInitialization());
  }
}
