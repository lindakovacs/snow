#pragma once

#include <string>

namespace Core
{
  class Application
  {
  public:
    Application();
    virtual ~Application();

    Application(const Application&) = delete;
    Application& operator = (const Application&) = delete;

    int Run();

  private:
    virtual void Execute() = 0;
  };
}
