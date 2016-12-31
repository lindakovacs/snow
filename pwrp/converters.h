#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace Pwrp
{
  std::string BinaryToBase64(const std::vector<char>& data);
  std::string BinaryToBase64(const std::string& data);

  void Utf16ToUtf8(const std::wstring& utf16, std::string& utf8);
  std::string Utf16ToUtf8(const std::wstring& utf16);

  void Utf8ToUtf16(const std::string& utf8, std::wstring& utf16);
  std::wstring Utf8ToUtf16(const std::string& utf8);

  void Utf8ToBase64(const std::string& utf8, std::string& base64);
  std::string Utf8ToBase64(const std::string& utf8);

  void Base64ToUtf8(const std::string& base64, std::string& utf8);
  std::string Base64ToUtf8(const std::string& base64);

  template <typename T>
  void FromString(const std::string& str, T& result)
  {
    std::istringstream iss(str);
    iss.exceptions(std::istream::failbit | std::istream::badbit);
    iss >> result;
  }

  inline void FromString(const std::string& str, std::string& result)
  {
    result = str;
  }

  template <typename T>
  T FromString(const std::string& str)
  {
    T result;
    FromString(str, result);
    return std::move(result);
  }

  template <typename T>
  void FromString(const std::wstring& str, T& result)
  {
    std::wistringstream iss(str);
    iss.exceptions(std::istream::failbit | std::istream::badbit);
    iss >> result;
  }

  inline void FromString(const std::wstring& str, std::wstring& result)
  {
    result = str;
  }

  template <typename T>
  T FromString(const std::wstring& str)
  {
    T result;
    FromString(str, result);
    return std::move(result);
  }
}
