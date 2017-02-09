#include "application.h"
#include "module.h"
#include "string_utils.h"
#include "tracer.h"

namespace Core
{
  Application::Application()
  {
  }

  Application::~Application()
  {
  }

  int Application::Run()
  {
    int code = EXIT_FAILURE;
    try
    {
      Core::CreateTracer(TraceLevel::All);
      CoreTrace(TraceLevel::Information, L"starting up");
      CoreTrace(TraceLevel::Information, Format(L"current process id %llu", GatherCurrentProcessId()));
      Execute();
      CoreTrace(TraceLevel::Information, L"shutting down");
      code = EXIT_SUCCESS;
    }
    catch (...)
    {
      CoreTraceError();
    }
    Core::CloseTracer();
    return code;
  }
}
