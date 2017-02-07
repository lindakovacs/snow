#include "io_queue_handler.h"
#include "socket_context.h"

#include <stdexcept>
#include <string>

#include <winsock2.h>
#include <windows.h>

namespace Network
{
  const bool g_bEndServer = false;

  int myprintf(const char *lpFormat, ...) // TODO: get rid
  {
    return 0;
  }

  void Routine(IoQueue::Sptr queue)
  {
    HANDLE hIOCP = queue->GetHandle();
    BOOL bSuccess = FALSE;
    int nRet = 0;
    OVERLAPPED* lpOverlapped = NULL;

    //PPER_SOCKET_CONTEXT lpPerSocketContext = NULL;
    //PPER_IO_CONTEXT lpIOContext = NULL;

    SocketContext::Sptr lpPerSocketContext = NULL;
    IoContext* lpIOContext = NULL;

    WSABUF buffRecv;
    WSABUF buffSend;
    DWORD dwRecvNumBytes = 0;
    DWORD dwSendNumBytes = 0;
    DWORD dwFlags = 0;
    DWORD dwIoSize = 0;

    while (1)
    {

      //
      // continually loop to service io completion packets
      //
      bSuccess = ::GetQueuedCompletionStatus(hIOCP, &dwIoSize, (PDWORD_PTR)&lpPerSocketContext, (LPOVERLAPPED*)&lpOverlapped, INFINITE);

      if (!bSuccess)
        myprintf("GetQueuedCompletionStatus() failed: %d\n", GetLastError());

      if (lpPerSocketContext == NULL) {

        //
        // CTRL-C handler used PostQueuedCompletionStatus to post an I/O packet with
        // a NULL CompletionKey (or if we get one for any reason).  It is time to exit.
        //
        return;
      }

      if (g_bEndServer)
      {
        //
        // main thread will do all cleanup needed - see finally block
        //
        return;
      }

      if (!bSuccess || (bSuccess && (dwIoSize == 0))) {

        //
        // client connection dropped, continue to service remaining (and possibly 
        // new) client connections
        //

   ///-->>> CloseClient(lpPerSocketContext, FALSE);

        continue;
      }

      //
      // determine what type of IO packet has completed by checking the PER_IO_CONTEXT 
      // associated with this socket.  This will determine what action to take.
      //
      lpIOContext = (IoContext*)lpOverlapped; // see structure

      switch (lpIOContext->IOOperation)
      {
      case ClientIoRead:

        //
        // a read operation has completed, post a write operation to echo the
        // data back to the client using the same data buffer.
        //
        lpIOContext->IOOperation = ClientIoWrite;
        lpIOContext->nTotalBytes = dwIoSize;
        lpIOContext->nSentBytes = 0;
        lpIOContext->wsabuf.len = dwIoSize;
        dwFlags = 0;

        nRet = WSASend(lpPerSocketContext->Socket, &lpIOContext->wsabuf, 1, &dwSendNumBytes, dwFlags, &(lpIOContext->Overlapped), NULL);

        if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()))
        {
          myprintf("WSASend() failed: %d\n", WSAGetLastError());

 ///-->>> CloseClient(lpPerSocketContext, FALSE);

        }

        // TODO: trace
        // myprintf("WorkerThread %d: Socket(%d) Recv completed (%d bytes), Send posted\n", ::GetCurrentThreadId(), lpPerSocketContext->Socket, dwIoSize);

        break;

      case ClientIoWrite:

        //
        // a write operation has completed, determine if all the data intended to be
        // sent actually was sent.
        //
        lpIOContext->IOOperation = ClientIoWrite;
        lpIOContext->nSentBytes += dwIoSize;
        dwFlags = 0;
        if (lpIOContext->nSentBytes < lpIOContext->nTotalBytes)
        {

          //
          // the previous write operation didn't send all the data,
          // post another send to complete the operation
          //
          buffSend.buf = lpIOContext->Buffer + lpIOContext->nSentBytes;
          buffSend.len = lpIOContext->nTotalBytes - lpIOContext->nSentBytes;
          nRet = WSASend(lpPerSocketContext->Socket, &buffSend, 1, &dwSendNumBytes, dwFlags, &(lpIOContext->Overlapped), NULL);

          if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()))
          {
            myprintf("WSASend() failed: %d\n", WSAGetLastError());

  ///-->>> CloseClient(lpPerSocketContext, FALSE);

          }

          // TRACE:
          myprintf("WorkerThread %d: Socket(%d) Send partially completed (%d bytes), Recv posted\n", GetCurrentThreadId(), lpPerSocketContext->Socket, dwIoSize);
        }
        else
        {

          //
          // previous write operation completed for this socket, post another recv
          //
          lpIOContext->IOOperation = ClientIoRead;
          dwRecvNumBytes = 0;
          dwFlags = 0;
          buffRecv.buf = lpIOContext->Buffer,
            buffRecv.len = MAX_BUFF_SIZE;
          nRet = WSARecv(lpPerSocketContext->Socket, &buffRecv, 1, &dwRecvNumBytes, &dwFlags, &(lpIOContext->Overlapped), NULL);

          if (nRet == SOCKET_ERROR && (ERROR_IO_PENDING != WSAGetLastError()))
          {
            myprintf("WSARecv() failed: %d\n", WSAGetLastError());

 ///-->>> CloseClient(lpPerSocketContext, FALSE);

          }

          // TRACE myprintf("WorkerThread %d: Socket(%d) Send completed (%d bytes), Recv posted\n", GetCurrentThreadId(), lpPerSocketContext->Socket, dwIoSize);
        }
        break;

      } //switch
    } //while
  }
}
