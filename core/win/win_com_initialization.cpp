#include "win_hr.h"
#include <core/com_initialization.h>
#include <atlcomcli.h>

namespace Core
{
  ComInitialization::ComInitialization()
  {
    HR(::CoInitializeEx(NULL, COINIT_MULTITHREADED));
    // HR(::CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL)); // TODO: investigate
  }

  ComInitialization::~ComInitialization()
  {
    ::CoUninitialize();
  };
}
