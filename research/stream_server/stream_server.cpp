#include "stream_server.h"

#include <core/thread_pool.h>
#include <network/interface_enumerator.h>
#include <network/io_queue.h>
#include <network/io_queue_handler.h>
#include <network/resource_initialization.h>
#include <network/socket.h>
#include <network/socket_context.h>

#include <chrono>
#include <thread>

// TODO: get rid
#include <winsock2.h>
#include <windows.h>

namespace Research
{
  void ThreadRoutine(Network::IoQueue::Sptr queue)
  {
    try
    {
      Network::Routine(queue);
    }
    catch (...)
    {
    }
  }

  struct Argument
  {
    Argument(std::vector<Network::Socket::Sptr>& listeningSockets)
      : ListeningSockets(listeningSockets)
    {
    }

    std::vector<Network::Socket::Sptr>& ListeningSockets;
    Network::IoQueue::Sptr Queue;
  };

  void AcceptRoutine(Argument& params)
  {
    try
    {
      Network::Socket::Sptr socket = params.ListeningSockets.front();
      while (1)
      {
        SOCKET sdAccept = ::WSAAccept(socket->GetHandle(), NULL, NULL, NULL, 0);
        if (sdAccept == SOCKET_ERROR)
        {
          throw 1;
        }

        Network::SocketContext::Sptr context(new Network::SocketContext());
        context->Socket = sdAccept;
        context->pIOContext.Overlapped.Internal = 0;
        context->pIOContext.Overlapped.InternalHigh = 0;
        context->pIOContext.Overlapped.Offset = 0;
        context->pIOContext.Overlapped.OffsetHigh = 0;
        context->pIOContext.Overlapped.hEvent = NULL;
        context->pIOContext.IOOperation = Network::ClientIoRead;
        context->pIOContext.nTotalBytes = 0;
        context->pIOContext.nSentBytes = 0;
        context->pIOContext.wsabuf.buf = context->pIOContext.Buffer; // ???
        context->pIOContext.wsabuf.len = sizeof(context->pIOContext.Buffer); // ???

        params.Queue->Push(context);

        DWORD dwRecvNumBytes = 0;
        DWORD dwFlags = 0;
        int nRet = WSARecv(sdAccept, &context->pIOContext.wsabuf, 1, &dwRecvNumBytes, &dwFlags, &context->pIOContext.Overlapped, NULL);
        if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()))
        {
          throw 2;
          //myprintf("WSARecv() Failed: %d\n", WSAGetLastError());
          //CloseClient(lpPerSocketContext, FALSE);
        }

      }
    }
    catch (...)
    {
    }
  }

  void StreamServer::Execute()
  {
    Network::ResourceInitialization::Sptr resource = Network::CreateResourceInitialization();
    Network::InterfaceEnumerator::Sptr enumerator = Network::CreateInterfaceEnumerator(L"localhost", L"6001");
    const std::vector<Network::AddressInformation> interfaces = enumerator->Gather();
    std::vector<Network::Socket::Sptr> listeningSockets = Network::CreateListeningSockets(interfaces);

    Network::IoQueue::Sptr queue(new Network::IoQueue());

    Core::ThreadPool::Uptr pool = Core::CreateNativeThreadPool();

    pool->Shedule("routine-01", std::bind(ThreadRoutine, std::cref(queue)));
    pool->Shedule("routine-02", std::bind(ThreadRoutine, std::cref(queue)));


    Argument params(listeningSockets);
    params.Queue = queue;

    pool->Shedule("routine-03", std::bind(AcceptRoutine, std::ref(params)));

    pool->Submit();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pool->Cancel();
    pool->Wait();
  }
}
