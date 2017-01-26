#pragma once

#include <memory>

namespace Core
{
  class Command
  {
  public:
    typedef std::shared_ptr<Command> Sptr;

    virtual ~Command() {}
    virtual void Execute() = 0;
  };
}
