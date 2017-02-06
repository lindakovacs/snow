#include "win_socket.h"

#include <stdexcept>
#include <string>

namespace Network
{
  WinSocket::WinSocket()
  {
    if (::WSAStartup(MAKEWORD(2, 2), &WsaData))
    {
      throw std::runtime_error("WSAStartup failed with error: " + std::to_string(::WSAGetLastError()));
    }
  }

  WinSocket::~WinSocket()
  {
    ::WSACleanup();
  }

  Socket::Sptr CreateSocket()
  {
    return Socket::Sptr(new WinSocket());
  }
}
