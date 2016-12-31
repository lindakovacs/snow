#include "converters.h"

#include <Poco/Base64Decoder.h>
#include <Poco/Base64Encoder.h>
#include <Poco/StreamCopier.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/UTF16Encoding.h>
#include <Poco/UTF8Encoding.h>
#include <Poco/TextConverter.h>

#include <cstdint>
#include <sstream>
#include <vector>

namespace Pwrp
{
  std::string BinaryToBase64(const std::vector<char>& data)
  {
    std::stringstream ss;
    Poco::Base64Encoder b64enc(ss);
    b64enc.write(data.data(), data.size());
    b64enc.close();
    return ss.str();
  }

  std::string BinaryToBase64(const std::string& data)
  {
    std::stringstream ss;
    Poco::Base64Encoder b64enc(ss);
    b64enc.write(data.data(), data.size());
    b64enc.close();
    return ss.str();
  }

  void Utf16ToUtf8(const std::wstring& utf16, std::string& utf8)
  {
    typedef std::wstring::value_type WStrChar;

    static_assert(sizeof(WStrChar) == 2 || sizeof(WStrChar) == 4, "Unexpected wchar_t size.");

    std::string tmp;

    if (!utf16.empty())
    {
      Poco::UTF16Encoding utf16Encoding;
      Poco::UTF8Encoding utf8Encoding;
      Poco::TextConverter converter(utf16Encoding, utf8Encoding);

      if (sizeof(WStrChar) == 4)
      {
        std::vector<uint16_t> buffer(utf16.begin(), utf16.end());
        converter.convert(buffer.data(), static_cast<int>(buffer.size() * sizeof(uint16_t)), tmp);
      }
      else
      {
        converter.convert(utf16.data(), static_cast<int>(utf16.size() * sizeof(WStrChar)), tmp);
      }
    }

    std::swap(utf8, tmp);
  }

  std::string Utf16ToUtf8(const std::wstring& utf16)
  {
    std::string utf8;
    Utf16ToUtf8(utf16, utf8);
    return std::move(utf8);
  }

  void Utf8ToUtf16(const std::string& utf8, std::wstring& utf16)
  {
    std::wstring tmp;
    Poco::UnicodeConverter::toUTF16(utf8, tmp);
    std::swap(utf16, tmp);
  }

  std::wstring Utf8ToUtf16(const std::string& utf8)
  {
    std::wstring utf16;
    Utf8ToUtf16(utf8, utf16);
    return std::move(utf16);
  }

  void Utf8ToBase64(const std::string& utf8, std::string& base64)
  {
    std::ostringstream oss;
    {
      Poco::Base64Encoder encoder(oss);
      encoder << utf8;
      encoder.flush();
    }
    base64 = oss.str();
  }

  std::string Utf8ToBase64(const std::string& utf8)
  {
    std::string base64;
    Utf8ToBase64(utf8, base64);
    return std::move(base64);
  }

  void Base64ToUtf8(const std::string& base64, std::string& utf8)
  {
    std::string tmp;
    std::istringstream iss(base64);
    Poco::Base64Decoder decoder(iss);
    Poco::StreamCopier::copyToString(decoder, tmp);
    std::swap(utf8, tmp);
  }

  std::string Base64ToUtf8(const std::string& base64)
  {
    std::string utf8;
    Base64ToUtf8(base64, utf8);
    return std::move(utf8);
  }
}
