#pragma once

#include <fstream>
#include <string>

namespace Core
{
  class File
  {
  public:
    static bool Exists(const std::wstring& fileName);
  };

  class FileReader
  {
  public:
    explicit FileReader(const std::wstring& fileName);
    FileReader(const FileReader&) = delete;
    FileReader& operator = (const FileReader&) = delete;
    ~FileReader();

    void Read(std::string& buffer);

  private:
    std::ifstream Stream;
  };

  class FileWriter
  {
  public:
    explicit FileWriter(const std::wstring& fileName);
    FileWriter(const FileWriter&) = delete;
    FileWriter& operator = (const FileWriter&) = delete;
    ~FileWriter();

    void Write(const std::string& buffer);

  private:
    std::ofstream Stream;
  };
}
