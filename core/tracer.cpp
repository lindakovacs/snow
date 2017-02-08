#include "file.h"
#include "module.h"
#include "string_utils.h"
#include "timestamp.h"
#include "tracer.h"

#include <mutex>

namespace Core
{
  Tracer::Sptr TracerInstance;
  std::mutex TracerInstanceGuard;

  class FileTracer: public Tracer
  {
  public:
    FileTracer(std::uint32_t levels, const std::wstring& fileName);
    virtual ~FileTracer();
    virtual void Set(std::uint32_t level, const std::wstring& message, const std::string& func);

  private:
    const std::uint32_t Levels;
    FileWriter Writer;
  };

  FileTracer::FileTracer(std::uint32_t levels, const std::wstring& fileName)
    : Levels(levels)
    , Writer(fileName)
  {
  }

  FileTracer::~FileTracer()
  {
  }

  void FileTracer::Set(std::uint32_t level, const std::wstring& message, const std::string& func)
  {
    if ((Levels & level) == 0)
    {
      return;
    }
    Writer.Write("[" + GetTimestamp() + "][" + Format("% 6u", GatherCurrentProcessId()) + "][" + std::to_string(level) + "][" + func + "] " + ToUtf8(message) + "\n");
  }

  void CreateTracer(std::uint32_t levels, const std::wstring& fileName)
  {
    std::lock_guard<std::mutex> lock(TracerInstanceGuard);
    TracerInstance.reset(new FileTracer(levels, fileName));
  }

  void CloseTracer()
  {
    std::lock_guard<std::mutex> lock(TracerInstanceGuard);
    TracerInstance.reset();
  }

  void Trace(std::uint32_t level, const std::wstring& message, const std::string& func)
  {
    std::lock_guard<std::mutex> lock(TracerInstanceGuard);
    if (TracerInstance)
    {
      TracerInstance->Set(level, message, func);
    }
  }

  std::wstring GetDefaultTraceFileName()
  {
    return GatherCurrentModulePathWithoutExt() + L"." + DefaultTraceFileNameExt;
  }
}
