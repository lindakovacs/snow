#include "file.h"
#include "string_utils.h"
#include <algorithm>

namespace Core
{
  bool File::Exists(const std::wstring& fileName)
  {
    std::ifstream stream(ToUtf8(fileName));
    const bool fileExists = stream.good();
    stream.close();
    return fileExists;
  }

  FileReader::FileReader(const std::wstring& fileName)
  : Stream(ToUtf8(fileName), std::ios::binary)
  {
  }

  FileReader::~FileReader()
  {
    Stream.close();
  }

  void FileReader::Read(std::string& buffer)
  {
    buffer.assign(std::istreambuf_iterator<char>(Stream), std::istreambuf_iterator<char>());
  }

  FileWriter::FileWriter(const std::wstring& fileName)
  : Stream(ToUtf8(fileName), std::ios::trunc)
  {
  }

  FileWriter::~FileWriter()
  {
    Stream.flush();
    Stream.close();
  }

  void FileWriter::Write(const std::string& buffer)
  {
    Stream.write(buffer.c_str(), buffer.size());
  }
}
