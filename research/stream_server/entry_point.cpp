#include "stream_server.h"
#include <build/version.h>
#include <core/tracer.h>

int main(int argc, char** argv)
{
  try
  {
    Research::TestStreamServer();
  }
  catch (...)
  {
    CoreTraceError();
  }
  return 0;
}
