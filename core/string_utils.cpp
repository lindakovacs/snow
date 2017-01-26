#include "string_utils.h"
#include <vector>

// TODO: gcc < 5 is not supported converters, investigate other way
#ifdef WIN32
#include <codecvt>
#endif

namespace Core
{
  std::list<std::wstring> Split(const std::wstring& multistring, const std::wstring& delimeters)
  {
    std::list<std::wstring> strings;
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
    return strings;
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
}
