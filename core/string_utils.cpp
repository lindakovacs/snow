#include "string_utils.h"

#include <cstdarg>
#include <cstdio>

// TODO: gcc < 5 is not supported converters, investigate other way
#ifdef WIN32
#include <codecvt>
#endif

#if defined (_MSC_VER) && (_MSC_VER < 1900)
// Visual Studio 2013, Visual Studio 2012, ...
#define SNPRINTF ::_snprintf_c
#define SNPRINTFW ::_snwprintf_c
#else
// Visual Studio 2015, GCC
#define SNPRINTF ::vsnprintf
#define SNPRINTFW ::vswprintf
#endif

namespace Core
{
  std::vector<std::wstring> Split(const std::wstring& multistring, const std::wstring& delimeters)
  {
    std::vector<std::wstring> strings;
    std::size_t start = 0;
    while (start < multistring.size())
    {
      std::size_t pos = multistring.find_first_of(delimeters, start);
      if (pos == std::wstring::npos)
      {
        pos = multistring.size();
      }
      const std::wstring& element = multistring.substr(start, pos - start);
      if (!element.empty())
      {
        strings.push_back(element);
      }
      start = pos + 1;
    }
    return std::move(strings);
  }

  std::wstring RemoveLastSegment(const std::wstring& source, const std::wstring& delimeters)
  {
    std::wstring target = source;
    const std::size_t pos = source.find_last_of(delimeters);
    if (pos == std::wstring::npos)
    {
      return std::move(target);
    }
    target.erase(pos, target.size() - pos);
    return std::move(target);
  }

  template<typename T1, typename T2>
  T1* ConvertFast(T1* target, int targetLen, const T2* source, int sourceLen)
  {
    if (targetLen > 0)
    {
      int index;
      for (index = 0; index < sourceLen && index < targetLen - 1; ++index)
      {
        target[index] = static_cast<T1>(source[index]);
      }
      target[index] = 0;
    }
    return target;
  }

  std::wstring ToWstring(const std::string& source)
  {
#ifdef WIN32
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(source);
#else
    std::vector<wchar_t> buffer(source.size() + 1, 0);
    return ConvertFast(&buffer.front(), buffer.size(), source.c_str(), source.size());
#endif
  }

  std::string ToUtf8(const std::wstring& source)
  {
#ifdef WIN32
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(source);
#else
    std::vector<char> buffer(source.size() + 1, 0);
    return ConvertFast(&buffer.front(), buffer.size(), source.c_str(), source.size());
#endif
  }

  std::string Format(const char* format, ...)
  {
    std::vector<char> buffer(256, 0);
    va_list arguments;
    va_start(arguments, format);

    while (SNPRINTF(&buffer.front(), buffer.size(), format, arguments) < 0)
    {
      buffer.resize(buffer.size() * 2, 0);
    }
    va_end(arguments);
    buffer.push_back(0);
    return &buffer.front();
  }

  std::wstring Format(const wchar_t* format, ...)
  {
    std::vector<wchar_t> buffer(256, 0);
    va_list arguments;
    va_start(arguments, format);

    while (SNPRINTFW(&buffer.front(), buffer.size(), format, arguments) < 0)
    {
      buffer.resize(buffer.size() * 2, 0);
    }
    va_end(arguments);
    buffer.push_back(0);
    return &buffer.front();
  }
}
