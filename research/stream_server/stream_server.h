#pragma once

#include <core/application.h>

namespace Research
{
  class StreamServer : public Core::Application
  {
  private:
    virtual void Execute();
  };
}
