#include "error.h"
#include "format_exception.h"
#include "string_utils.h"

namespace Core
{
  std::wstring FormatException(std::exception_ptr error)
  {
    if (error == nullptr)
    {
      return L"exception is null";
    }

    std::wstring result = L"exception is not defined";
    try
    {
      try
      {
        std::rethrow_exception(error);
      }
      catch (const Error& err)
      {
        result = err.GetMessage();
      }
      catch (const std::exception& err)
      {
        result = ToWstring(err.what());
      }
      catch (...)
      {
        result = L"unknown exception (maybe forgotten to add the formatting for exception)";
      }
    }
    catch (...)
    {
      result = L"by catching an exception other problem occurred, please track down";
    }

    return result;
  }
}
