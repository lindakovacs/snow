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
    virtual void DisableSendBuffering() = 0;
    virtual void ForceSubsequent() = 0; // force the subsequent closesocket to be abortative.

    virtual std::uint64_t GetHandle() = 0;
  };

  std::vector<Socket::Sptr> CreateListeningSockets(const std::vector<AddressInformation>& interfaces);
}
