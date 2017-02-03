#include "stream_client.h"
#include <build/version.h>
#include <core/tracer.h>

int main(int argc, char** argv)
{
  try
  {
    Research::TestStreamClient();
  }
  catch (...)
  {
    CoreTraceError();
  }
  return 0;
}
