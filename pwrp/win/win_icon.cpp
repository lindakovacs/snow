#include "win_icon.h"
#include <libs/core/win/win_handle.h>
#include <libs/core/win/win_hr.h>
#include <libs/core/win/win_image.h>
#include <libs/pwrp/converters.h>

#include <atlcomcli.h>
#include <wincodec.h>

#include <shellapi.h>
#include <shlobj.h>
#include <commoncontrols.h>

namespace Pwrp
{
  namespace
  {
    // https://blogs.msdn.microsoft.com/oldnewthing/20140120-00/?p=2043 
    int GetIconIndex(const std::wstring& fileName)
    {
      SHFILEINFOW sfi = {};
      ::SHGetFileInfoW(fileName.c_str(), 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX);
      return sfi.iIcon;
    }

    HICON GetIcon(int iconIndex, int iconSize)
    {
      CComPtr<IImageList> spiml;
      Core::HR(::SHGetImageList(iconSize, IID_PPV_ARGS(&spiml))); // SHIL_LARGE (32x32)  SHIL_JUMBO
      HICON hico = 0;
      Core::HR(spiml->GetIcon(iconIndex, ILD_TRANSPARENT, &hico));
      return hico;
    }

    //
    // ICONS (.ICO type 1) are structured like this:
    //
    // ICONHEADER (just 1)
    // ICONDIR [1...n] (an array, 1 for each image)
    // [BITMAPINFOHEADER+COLOR_BITS+MASK_BITS] [1...n] (1 after the other, for each image)
    //
    // CURSORS (.ICO type 2) are identical in structure, but use
    // two monochrome bitmaps (real XOR and AND masks, this time).
    //
    typedef struct
    {
      WORD idReserved; // must be 0
      WORD idType; // 1 = ICON, 2 = CURSOR
      WORD idCount; // number of images (and ICONDIRs)

      // ICONDIR [1...n]
      // ICONIMAGE [1...n]

    } ICONHEADER;

    // An array of ICONDIRs immediately follow the ICONHEADER
    typedef struct
    {
      BYTE bWidth;
      BYTE bHeight;
      BYTE bColorCount;
      BYTE bReserved;
      WORD wPlanes; // for cursors, this field = wXHotSpot
      WORD wBitCount; // for cursors, this field = wYHotSpot
      DWORD dwBytesInRes;
      DWORD dwImageOffset; // file-offset to the start of ICONIMAGE

    } ICONDIR;

    //
    // After the ICONDIRs follow the ICONIMAGE structures -
    // consisting of a BITMAPINFOHEADER, (optional) RGBQUAD array, then
    // the color and mask bitmap bits (all packed together
    //
    typedef struct
    {
      BITMAPINFOHEADER biHeader; // header for color bitmap (no mask header)
      //RGBQUAD rgbColors[1...n];
      //BYTE bXOR[1]; // DIB bits for color bitmap
      //BYTE bAND[1]; // DIB bits for mask bitmap

    } ICONIMAGE;

    // Write the ICO header to disk
    UINT WriteIconHeader(HANDLE hFile, int nImages)
    {
      ICONHEADER iconheader = {};
      DWORD nWritten = 0;

      // Setup the icon header
      iconheader.idReserved = 0; // Must be 0
      iconheader.idType = 1; // Type 1 = ICON (type 2 = CURSOR)
      iconheader.idCount = nImages; // number of ICONDIRs

      // Write the header to disk
      WriteFile( hFile, &iconheader, sizeof(iconheader), &nWritten, 0);

      // following ICONHEADER is a series of ICONDIR structures (idCount of them, in fact)
      return nWritten;
    }

    // Return the number of BYTES the bitmap will take ON DISK
    UINT NumBitmapBytes(BITMAP *pBitmap)
    {
      int nWidthBytes = pBitmap->bmWidthBytes;

      // bitmap scanlines MUST be a multiple of 4 bytes when stored
      // inside a bitmap resource, so round up if necessary
      if(nWidthBytes & 3)
        nWidthBytes = (nWidthBytes + 4) & ~3;

      return nWidthBytes * pBitmap->bmHeight;
    }

    // Return number of bytes written
    UINT WriteIconImageHeader(HANDLE hFile, BITMAP *pbmpColor, BITMAP *pbmpMask)
    {
      BITMAPINFOHEADER biHeader = {};
      DWORD nWritten = 0;
      UINT nImageBytes = 0;

      // calculate how much space the COLOR and MASK bitmaps take
      nImageBytes = NumBitmapBytes(pbmpColor) + NumBitmapBytes(pbmpMask);

      // write the ICONIMAGE to disk (first the BITMAPINFOHEADER)
      ZeroMemory(&biHeader, sizeof(biHeader));

      // fill in only those fields that are necessary
      biHeader.biSize = sizeof(biHeader);
      biHeader.biWidth = pbmpColor->bmWidth;
      biHeader.biHeight = pbmpColor->bmHeight * 2; // height of color+mono
      biHeader.biPlanes = pbmpColor->bmPlanes;
      biHeader.biBitCount = pbmpColor->bmBitsPixel;
      biHeader.biSizeImage = nImageBytes;

      // write the BITMAPINFOHEADER
      WriteFile(hFile, &biHeader, sizeof(biHeader), &nWritten, 0);

      // write the RGBQUAD color table (for 16 and 256 colour icons)
      if(pbmpColor->bmBitsPixel == 2 || pbmpColor->bmBitsPixel == 8)
      {
        // TODO: investigate
      }
      return nWritten;
    }

    // Wrapper around GetIconInfo and GetObject(BITMAP)
    BOOL GetIconBitmapInfo(HICON hIcon, ICONINFO* pIconInfo, BITMAP* pbmpColor, BITMAP* pbmpMask)
    {
      if(!GetIconInfo(hIcon, pIconInfo))
        return FALSE;

      if(!GetObjectW(pIconInfo->hbmColor, sizeof(BITMAP), pbmpColor))
        return FALSE;

      if(!GetObjectW(pIconInfo->hbmMask, sizeof(BITMAP), pbmpMask))
        return FALSE;

      return TRUE;
    }

    // Write one icon directory entry - specify the index of the image
    UINT WriteIconDirectoryEntry(HANDLE hFile, int nIdx, HICON hIcon, UINT nImageOffset)
    {
      ICONINFO iconInfo = {};
      ICONDIR iconDir = {};

      BITMAP bmpColor;
      BITMAP bmpMask;

      DWORD nWritten = 0;
      UINT nColorCount = 0;
      UINT nImageBytes = 0;

      GetIconBitmapInfo(hIcon, &iconInfo, &bmpColor, &bmpMask);

      nImageBytes = NumBitmapBytes(&bmpColor) + NumBitmapBytes(&bmpMask);

      if(bmpColor.bmBitsPixel >= 8)
        nColorCount = 0;
      else
        nColorCount = 1 << (bmpColor.bmBitsPixel * bmpColor.bmPlanes);

      // Create the ICONDIR structure
      iconDir.bWidth = (BYTE)bmpColor.bmWidth;
      iconDir.bHeight = (BYTE)bmpColor.bmHeight;
      iconDir.bColorCount = nColorCount;
      iconDir.bReserved = 0;
      iconDir.wPlanes = bmpColor.bmPlanes;
      iconDir.wBitCount = bmpColor.bmBitsPixel;
      iconDir.dwBytesInRes = sizeof(BITMAPINFOHEADER) + nImageBytes;
      iconDir.dwImageOffset = nImageOffset;

      // Write to disk
      WriteFile(hFile, &iconDir, sizeof(iconDir), &nWritten, 0);

      // Free resources
      DeleteObject(iconInfo.hbmColor);
      DeleteObject(iconInfo.hbmMask);

      return nWritten;
    }

    UINT WriteIconData(HANDLE hFile, HBITMAP hBitmap)
    {
      BITMAP bmp = {};
      int index = 0;
      BYTE* pIconData = 0;

      UINT nBitmapBytes = 0;
      DWORD nWritten = 0;

      GetObjectW(hBitmap, sizeof(BITMAP), &bmp);
      nBitmapBytes = NumBitmapBytes(&bmp);
      pIconData = (BYTE*)malloc(nBitmapBytes);

      GetBitmapBits(hBitmap, nBitmapBytes, pIconData);

      // bitmaps are stored inverted (vertically) when on disk..
      // so write out each line in turn, starting at the bottom + working
      // towards the top of the bitmap. Also, the bitmaps are stored in packed
      // in memory - scanlines are NOT 32bit aligned, just 1-after-the-other
      for (index = bmp.bmHeight - 1; index >= 0; index--)
      {
        // Write the bitmap scanline
        WriteFile(
          hFile,
          pIconData + (index * bmp.bmWidthBytes), // calculate offset to the line
          bmp.bmWidthBytes, // 1 line of BYTES
          &nWritten,
          0);

        // extend to a 32bit boundary (in the file) if necessary
        if(bmp.bmWidthBytes & 3)
        {
          DWORD padding = 0;
          WriteFile(hFile, &padding, 4 - bmp.bmWidthBytes, &nWritten, 0);
        }
      }

      free(pIconData);
      return nBitmapBytes;
    }

    // Create a .ICO file, using the specified array of HICON images
    BOOL SaveIconToFile(const char* tempIconFile, HICON hIcon[], int nNumIcons)
    {
      int index = 0;
      int* pImageOffset = 0;

      if(hIcon == 0 || nNumIcons < 1)
        return FALSE;

      // Save icon to disk:
      HANDLE hFile = ::CreateFileA(tempIconFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

      if(hFile == INVALID_HANDLE_VALUE)
        return FALSE;

      WriteIconHeader(hFile, nNumIcons); // Write the iconheader first of all
      ::SetFilePointer(hFile, sizeof(ICONDIR) * nNumIcons, 0, FILE_CURRENT); // Leave space for the IconDir entries

      pImageOffset = (int*)malloc(nNumIcons * sizeof(int));

      // Now write the actual icon images!
      for (index = 0; index < nNumIcons; index++)
      {
        ICONINFO iconInfo = {};
        BITMAP bmpColor = {};
        BITMAP bmpMask = {};

        GetIconBitmapInfo(hIcon[index], &iconInfo, &bmpColor, &bmpMask);

        // record the file-offset of the icon image for when we write the icon directories
        pImageOffset[index] = SetFilePointer(hFile, 0, 0, FILE_CURRENT);

        // bitmapinfoheader + colortable
        WriteIconImageHeader(hFile, &bmpColor, &bmpMask);

        // color and mask bitmaps
        WriteIconData(hFile, iconInfo.hbmColor);
        WriteIconData(hFile, iconInfo.hbmMask);

        DeleteObject(iconInfo.hbmColor);
        DeleteObject(iconInfo.hbmMask);
      }

      // Lastly, skip back and write the icon directories.
      ::SetFilePointer(hFile, sizeof(ICONHEADER), 0, FILE_BEGIN);

      for (index = 0; index < nNumIcons; index++)
      {
        WriteIconDirectoryEntry(hFile, index, hIcon[index], pImageOffset[index]);
      }

      free(pImageOffset);
      ::CloseHandle(hFile);
      return TRUE;
    }

    void ConvertImageFile(const std::wstring& sourceFile, std::vector<unsigned char>& targetBuffer, const GUID& format)
    {
      Core::ImagingFactory::Sptr factory(new Core::ImagingFactory());
      CComPtr<IWICStream> sourceStream = factory->CreateReadFileStream(sourceFile);
      const Core::ImagingFactory::Statistic& stat = factory->GetStreamStatistic(sourceStream);
      targetBuffer.resize(stat.Size, 0);
      CComPtr<IWICStream> targetStream = factory->CreateStreamFromMemory(targetBuffer);
      Core::Image image(sourceStream, factory);
      image.To(targetStream, format);
    }
  }

  WinIcon::WinIcon(const std::string& targetPath)
  : TargetPath(targetPath)
  , IconIndex(GetIconIndex(Utf8ToUtf16(targetPath)))
  {
  }

  WinIcon::~WinIcon()
  {
  }

  void WinIcon::GetIconBysize(std::vector<unsigned char>& buffer, int iconSize) const
  {
    const std::string& tempIconFile = TempFile.path();
    HICON hicon = GetIcon(IconIndex, iconSize);
    Core::HiconUptr handle(hicon);
    if (!SaveIconToFile(tempIconFile.c_str(), &hicon, 1))
    {
      throw std::logic_error("Can't save icon to file: " + tempIconFile);
    }
    ConvertImageFile(Utf8ToUtf16(tempIconFile), buffer, CLSID_WICPngEncoder);
  }

  std::string WinIcon::GetLarge() const
  {
    std::vector<unsigned char> buffer;
    GetIconBysize(buffer, SHIL_LARGE);
    return buffer.empty() ? std::string() : std::string(reinterpret_cast<const char*>(&buffer.front()), buffer.size());
  }

  std::string WinIcon::GetJumbo() const
  {
    std::vector<unsigned char> buffer;
    GetIconBysize(buffer, SHIL_JUMBO);
    return buffer.empty() ? std::string() : std::string(reinterpret_cast<const char*>(&buffer.front()), buffer.size());
  }

  Icon::Sptr CreateIcon(const std::string& path)
  {
    return Icon::Sptr(new WinIcon(path));
  }
}
