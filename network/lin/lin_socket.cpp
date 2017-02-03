#include "lin_socket.h"

namespace Network
{
  LinSocket::LinSocket()
  {
  }

  LinSocket::~LinSocket()
  {
  }

  Socket::Sptr CreateSocket()
  {
    return Socket::Sptr(new LinSocket());
  }
}
