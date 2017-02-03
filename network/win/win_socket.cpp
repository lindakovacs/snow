#include "win_socket.h"

#include <stdexcept>
#include <string>

namespace Network
{
  WinSocket::WinSocket()
  {
    const int error = ::WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (error != 0)
    {
      throw std::runtime_error("WSAStartup failed, error: " + std::to_string(error));
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
