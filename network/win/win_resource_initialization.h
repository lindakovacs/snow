#pragma once

#include <network/resource_initialization.h>

#include <winsock2.h>
#include <windows.h>

namespace Network
{
  class WinResourceInitialization : public ResourceInitialization
  {
  public:
    WinResourceInitialization();
    virtual ~WinResourceInitialization();

  private:
    WSADATA WsaData;
  };
}
