#pragma once

#include <string>

namespace Core
{
  class Error
  {
  public:
    explicit Error(const std::wstring& message = std::wstring());
    std::wstring GetMessage() const;

  private:
    const std::wstring Message;
  };
}
