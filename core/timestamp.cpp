#include "timestamp.h"

#include <ctime>
#include <sstream>
#include <vector>

namespace Core
{
  std::string FormatTimeString(unsigned year, unsigned month, unsigned day, unsigned hour, unsigned minute, unsigned second, unsigned milliseconds)
  {
    std::vector<char> buffer(1024, 0);
    while (::snprintf(&buffer.front(), buffer.size(), "%04u.%02u.%02u %02u:%02u:%02u.%03u", year, month, day, hour, minute, second, milliseconds) < 0)
    {
      buffer.resize(buffer.size() * 2, 0); // TODO: check max size
    }
    buffer.push_back(0);
    return std::string(&buffer.front());
  }

  long long GetTimePartInMilliseconds(const std::chrono::system_clock::time_point& timePoint)
  {
    const auto timeSinceEpoch = timePoint.time_since_epoch();
    const auto timeInMsec = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceEpoch);
    const auto timeInSec = std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch);
    return timeInMsec.count() - timeInSec.count() * 1000;
  }

  std::string GetTimestamp(const std::chrono::system_clock::time_point& timePoint)
  {
    const time_t current = std::chrono::system_clock::to_time_t(timePoint);
    // TODO: see localtime_s(&detailedTime, &current);
    // std::put_time(&detailedTime, "%Y-%m-%d %H:%M:%S");
    struct tm* detailedTime = localtime(&current);
    return FormatTimeString(detailedTime->tm_year + 1900, detailedTime->tm_mon + 1, detailedTime->tm_mday, detailedTime->tm_hour, detailedTime->tm_min, detailedTime->tm_sec, static_cast<unsigned>(GetTimePartInMilliseconds(timePoint)));
  }
}
