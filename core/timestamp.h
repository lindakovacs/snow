#pragma once

#include <chrono>
#include <string>

namespace Core
{
  std::string GetTimestamp(const std::chrono::system_clock::time_point& timePoint = std::chrono::system_clock::now());
}
