#pragma once

#include <libs/pwrp/icon.h>
#include <Poco/TemporaryFile.h>

namespace Pwrp
{
  class WinIcon: public Icon
  {
  public:
    explicit WinIcon(const std::string& targetPath);
    virtual ~WinIcon();
    virtual std::string GetLarge() const;
    virtual std::string GetJumbo() const;

  private:
    void GetIconBysize(std::vector<unsigned char>& buffer, int iconSize) const;

  private:
    const std::string TargetPath;
    const int IconIndex;
    Poco::TemporaryFile TempFile;
  };
}
