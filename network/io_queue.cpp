#include "io_queue.h"

#include <stdexcept>
#include <string>

namespace Network
{
  IoQueue::IoQueue()
    : Handle(::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0))
  {
    if (!Handle)
    {
      throw std::runtime_error("CreateIoCompletionPort (like create) failed with error: " + std::to_string(::GetLastError()));
    }
  }

  IoQueue::~IoQueue()
  {
    if (Handle)
    {
      ::CloseHandle(Handle);
    }
  }

  void IoQueue::PostCompletionStatus()
  {
    ::PostQueuedCompletionStatus(Handle, 0, 0, NULL);
  }

  HANDLE IoQueue::GetHandle() const
  {
    return Handle;
  }

  void IoQueue::Push(SocketContext::Sptr socketContext)
  {
    Handle = CreateIoCompletionPort((HANDLE)(socketContext->Socket), Handle, (DWORD_PTR)socketContext.get(), 0); // TODO
    if (!Handle)
    {
      throw std::runtime_error("CreateIoCompletionPort (like add) failed with error: " + std::to_string(::GetLastError()));
    }

    Contexts.push_back(socketContext);
  }
}
