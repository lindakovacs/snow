#include <core/module.h>
#include <cstdint>
#include <windows.h>

namespace Core
{
  std::wstring GatherCurrentModulePath()
  {
    const std::int32_t size = 10 * 1024;
    wchar_t buffer[size] = {};
    const std::int32_t bytesPlaced = ::GetModuleFileNameW(NULL, buffer, size - 1);
    // TODO: handle error
    return buffer;
  }
}
