#include "test_thread_pool.h"
#include <build/version.h>
#include <core/tracer.h>

int main(int argc, char** argv)
{
  try
  {
    Core::TestThreadPool();
  }
  catch (...)
  {
    CoreTraceError();
  }
  return 0;
}
