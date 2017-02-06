#pragma once

#include <network/socket.h>

#include <winsock2.h>
#include <windows.h>

namespace Network
{
  class WinSocket: public Socket
  {
  public:
    explicit WinSocket(SOCKET handle);
    virtual ~WinSocket();
    virtual void EnableNonBlockingMode();

  private:
    SOCKET Handle;
  };
}
