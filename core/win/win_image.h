#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <atlcomcli.h>
#include <wincodec.h>
#include <windows.h>

namespace Core
{
  class ImagingFactory
  {
  public:
    typedef std::shared_ptr<ImagingFactory> Sptr;

    ImagingFactory();
    ~ImagingFactory();

    struct Statistic
    {
      std::uint64_t Size;
    };

    Statistic GetStreamStatistic(CComPtr<IWICStream> stream) const;
    std::uint64_t GetCurrentPosition(CComPtr<IWICStream> stream) const;

    CComPtr<IWICStream> CreateStreamFromMemory(std::vector<unsigned char>& buffer) const;
    CComPtr<IWICStream> CreateReadFileStream(const std::wstring& fileName) const;
    CComPtr<IWICStream> CreateWriteFileStream(const std::wstring& fileName) const;

    CComPtr<IWICBitmapDecoder> CreateDecoder(CComPtr<IWICStream> readStream) const;
    CComPtr<IWICBitmapEncoder> CreateEncoder(CComPtr<IWICStream> writeStream, const GUID& format) const;

    CComPtr<IWICBitmapScaler> CreateScaler() const;

  private:
    CComPtr<IWICStream> CreateFileStream(const std::wstring& fileName, std::uint32_t desiredAccess) const;

  private:
    CComPtr<IWICImagingFactory> Instance;
  };

  class Image
  {
  public:
    typedef std::shared_ptr<Image> Sptr;
    typedef std::unique_ptr<Image> Uptr;

    Image(CComPtr<IWICStream> source, ImagingFactory::Sptr factory);

    void To(CComPtr<IWICStream> target, const GUID& format) const;
    void ToFrame(CComPtr<IWICStream> target, const GUID& format, UINT frameWidth, UINT frameHeight, bool fitToFrame) const;

  private:
    void TransformFrame(UINT frameIndex, IWICBitmapDecoder& decoder, IWICBitmapEncoder& encoder) const;
    void TransformFrame(UINT frameIndex, IWICBitmapDecoder& decoder, IWICBitmapEncoder& encoder, UINT frameWidth, UINT frameHeight, bool fitToFrame) const;

  private:
    CComPtr<IWICStream> Source;
    ImagingFactory::Sptr Factory;
  };
}
