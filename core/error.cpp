#include "error.h"

namespace Core
{
  Error::Error(const std::wstring& message)
  : Message(message)
  {
  }

  Error::~Error()
  {
  }

  std::wstring Error::GetMessage() const
  {
    return Message;
  }
}
