#include "stream_server.h"

#include <network/socket.h>

namespace Research
{
  void TestStreamServer()
  {
    Network::Socket::Sptr socket = Network::CreateSocket();
  }
}
