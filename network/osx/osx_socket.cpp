#include "osx_socket.h"

namespace Network
{
  OsxSocket::OsxSocket()
  {
  }

  OsxSocket::~OsxSocket()
  {
  }

  Socket::Sptr CreateSocket()
  {
    return Socket::Sptr(new OsxSocket());
  }
}
