#pragma once

#include <memory>
#include <vector>

#include <winsock2.h>
#include <windows.h>
#include <mswsock.h>

#define MAX_BUFF_SIZE 8192

namespace Network
{
  enum IoOperation
  {
    ClientIoAccept,
    ClientIoRead,
    ClientIoWrite
  };

  struct IoContext
  {
    WSAOVERLAPPED               Overlapped;
    char                        Buffer[MAX_BUFF_SIZE];
    WSABUF                      wsabuf;
    int                         nTotalBytes;
    int                         nSentBytes;
    IoOperation                 IOOperation;
  };

  struct SocketContext
  {
    typedef std::shared_ptr<SocketContext> Sptr;

    SOCKET Socket;
    IoContext pIOContext;
  };

  typedef std::vector<SocketContext::Sptr> SocketContextList;
}
