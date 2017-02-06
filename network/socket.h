#pragma once

#include "interface_enumerator.h"

namespace Network
{
  class Socket
  {
  public:
    typedef std::shared_ptr<Socket> Sptr;
    virtual ~Socket() {}
    virtual void EnableNonBlockingMode() = 0;
  };

  std::vector<Socket::Sptr> CreateListeningSockets(const std::vector<AddressInformation>& interfaces);
}
