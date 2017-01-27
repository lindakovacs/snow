#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace Core
{
  // Split items by char delimeters
  // Example (delimeter is '|'):
  // 'ABC|DE||F|||' -> 'ABC','DE','F'
  // 'ABC' -> 'ABC'
  // '|||' -> empty list
  // '' -> empty list
  std::vector<std::wstring> Split(const std::wstring& multistring, const std::wstring& delimeters);

  // Example (delimeter is "\\/"):
  // /user/vika/books/never_ending_story.pdf -> /user/vika/books
  std::wstring RemoveLastSegment(const std::wstring& source, const std::wstring& delimeters);

  std::wstring ToWstring(const std::string& source);
  std::string ToUtf8(const std::wstring& source);

  template <typename T>
  void FromString(const std::wstring& source, T& result)
  {
    std::wistringstream iss(source);
    iss.exceptions(std::istream::failbit | std::istream::badbit);
    iss >> result;
  }

  template <typename T>
  T FromString(const std::wstring& source)
  {
    T result;
    FromString(source, result);
    return result;
  }
}
