#pragma once

#include <network/socket.h>

#include <winsock2.h>
#include <windows.h>

namespace Network
{
  class WinSocket: public Socket
  {
  public:
    WinSocket();
    ~WinSocket();

  private:
    WSADATA WsaData;
  };
}
