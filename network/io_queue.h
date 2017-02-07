#pragma once

#include "socket_context.h"

namespace Network
{
  class IoQueue
  {
  public:
    typedef std::shared_ptr<IoQueue> Sptr;

    IoQueue();
    ~IoQueue();

    HANDLE GetHandle() const;

    // CTRL-C handler used PostQueuedCompletionStatus to post an I/O packet with
    // a NULL CompletionKey (or if we get one for any reason).  It is time to exit.

    // Cause worker threads to exit
    void PostCompletionStatus();
    void Push(SocketContext::Sptr socketContext);

  private:
    HANDLE Handle;

    SocketContextList Contexts; // TODO: ???
  };
}
