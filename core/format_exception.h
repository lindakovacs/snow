#pragma once

#include <exception>
#include <string>

namespace Core
{
  std::wstring FormatException(std::exception_ptr error);
}
