#pragma once

#include <cstdint>
#include <string>

namespace Core
{
  std::wstring GatherCurrentModulePath();
  std::wstring GatherCurrentModulePathWithoutExt();
  std::uint32_t GatherCurrentProcessId();
}
