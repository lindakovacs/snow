#pragma once

#include <string>

namespace Core
{
  class Error
  {
  public:
    explicit Error(const std::wstring& message = std::wstring());
    Error(const Error&) = delete;
    Error& operator=(const Error&) = delete;
    ~Error();

    std::wstring GetMessage() const;

  private:
    const std::wstring Message;
  };
}
