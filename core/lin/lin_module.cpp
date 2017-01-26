#include <core/module.h>
#include <core/string_utils.h>
#include <cstdint>
#include <unistd.h>

namespace Core
{
  std::wstring GatherCurrentModulePath()
  {
    char link[1024] = {};
    sprintf(link, "/proc/%d/exe", ::getpid());
    const std::int32_t size = 10 * 1024;
    char buffer[size] = {};
    const std::int32_t bytesPlaced = ::readlink(link, buffer, size - 1);
    // TODO: handle error
    return ToWstring(buffer);
  }
}
