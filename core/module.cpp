#include "module.h"
#include "string_utils.h"

namespace Core
{
  std::wstring GatherCurrentModulePathWithoutExt()
  {
    return RemoveLastSegment(GatherCurrentModulePath(), L".");
  }
}
