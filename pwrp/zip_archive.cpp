#include "zip_archive.h"
#include <Poco/Delegate.h>
#include <Poco/File.h>
#include <Poco/Zip/Compress.h>
#include <fstream>

namespace Pwrp
{
  void CreateZipArchive(const std::string& source, const std::string& target)
  {
    std::ofstream out(target.c_str(), std::ios::binary);
    Poco::Zip::Compress c(out, true);
    //c.EDone += Poco::Delegate<ZipApp, const Poco::Zip::ZipLocalFileHeader>(this, &ZipApp::onDone);
    Poco::File file(source);
    if (!file.exists())
    {
      // TRACE:
      return;
    }
    Poco::Path anEntry(source);
    //c.addFile(anEntry, anEntry.getFileName(), Poco::Zip::ZipCommon::CM_DEFLATE, Poco::Zip::ZipCommon::CL_NORMAL);
    c.addFile(anEntry, anEntry.getFileName());

    c.close();
    out.close();
  }
}
