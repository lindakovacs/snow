#include "application.h"
#include "test_thread_pool.h"

#include <core/tracer.h>

namespace Research
{
  void TestApplication::Execute()
  {
    TestThreadPool();
    CoreTrace(TraceLevel::Information, L"test thread pool completed");
  }
}
