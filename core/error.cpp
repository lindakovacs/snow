#include "error.h"

namespace Core
{
  Error::Error(const std::wstring& message)
  : Message(message)
  {
  }

  std::wstring Error::GetMessage() const
  {
    return Message;
  }
}
