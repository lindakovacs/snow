#include "stream_server.h"
#include <build/version.h>
#include <core/tracer.h>

int main(int argc, char** argv)
{
  int code = EXIT_FAILURE;
  try
  {
    Core::CreateTracer(TraceLevel::All);
    CoreTrace(TraceLevel::Information, L"starting up");

    Research::TestStreamServer();

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
