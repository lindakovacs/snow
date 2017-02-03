#pragma once

#include <memory>

namespace Network
{
  class Socket
  {
  public:
    typedef std::shared_ptr<Socket> Sptr;
  
  };

  Socket::Sptr CreateSocket();
}
