#pragma once

#include "socket.h"

namespace Network
{
  class SocketHandler
  {
  public:
    typedef std::shared_ptr<SocketHandler> Sptr;

    virtual ~SocketHandler() {}

    virtual void Register(Socket::Sptr socket) = 0;
    virtual void Unregister(Socket::Sptr socket) = 0;
  };

  SocketHandler::Sptr CreateSocketHandler(std::uint32_t threadsNumber);
}
