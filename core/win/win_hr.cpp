#include "win_hr.h"
#include <exception>
#include <string>

namespace Core
{
  void HR(HRESULT error)
  {
    if (FAILED(error))
    {
      const std::string message = "Failed HRESULT: " + std::to_string(error);
      throw std::logic_error(message);
    }
  }
}
