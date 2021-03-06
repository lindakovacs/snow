#pragma once

#include "format_exception.h"

#include <cstdint>
#include <memory>

namespace TraceLevel
{
  const std::uint32_t Information = 1;
  const std::uint32_t Warning = 2;
  const std::uint32_t Error = 4;
  const std::uint32_t All = Information | Warning | Error;
}

namespace Core
{
  class Tracer
  {
  public:
    typedef std::shared_ptr<Tracer> Sptr;
    virtual ~Tracer() {}
    virtual void Set(std::uint32_t level, const std::wstring& message, const std::string& func) = 0;
  };

  const std::wstring DefaultTraceFileNameExt = L"diagnostics";
  std::wstring GetDefaultTraceFileName();

  void CreateTracer(std::uint32_t levels, const std::wstring& fileName = GetDefaultTraceFileName());
  void CloseTracer();
  void Trace(std::uint32_t level, const std::wstring& message, const std::string& func);
}

#define CoreTrace(level, message) ::Core::Trace(level, message, __FUNCTION__);
#define CoreTraceError() CoreTrace(::TraceLevel::Error, ::Core::FormatException(std::current_exception()));
