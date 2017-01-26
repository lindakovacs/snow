#include "win_hr.h"
#include "win_image.h"

namespace Core
{
  ImagingFactory::ImagingFactory()
  {
    HR(Instance.CoCreateInstance(CLSID_WICImagingFactory));
  }

  ImagingFactory::~ImagingFactory()
  {
  }

  ImagingFactory::Statistic ImagingFactory::GetStreamStatistic(CComPtr<IWICStream> stream) const
  {
    STATSTG rawStat = {};
    HR(stream->Stat(&rawStat, STATFLAG_DEFAULT));
    Statistic stat = {};
    stat.Size = rawStat.cbSize.QuadPart;
    return stat;
  }

  std::uint64_t ImagingFactory::GetCurrentPosition(CComPtr<IWICStream> stream) const
  {
    LARGE_INTEGER displacement = {};
    ULARGE_INTEGER origin = {};
    HR(stream->Seek(displacement, STREAM_SEEK_CUR, &origin));
    return origin.QuadPart;
  }

  CComPtr<IWICStream> ImagingFactory::CreateStreamFromMemory(std::vector<unsigned char>& buffer) const
  {
    CComPtr<IWICStream> stream;
    HR(Instance->CreateStream(&stream));
    HR(stream->InitializeFromMemory(&buffer.front(), static_cast<DWORD>(buffer.size())));
    return stream;
  }

  CComPtr<IWICStream> ImagingFactory::CreateReadFileStream(const std::wstring& fileName) const
  {
    return CreateFileStream(fileName, GENERIC_READ);
  }

  CComPtr<IWICStream> ImagingFactory::CreateWriteFileStream(const std::wstring& fileName) const
  {
    return CreateFileStream(fileName, GENERIC_WRITE);
  }

  CComPtr<IWICStream> ImagingFactory::CreateFileStream(const std::wstring& fileName, std::uint32_t desiredAccess) const
  {
    CComPtr<IWICStream> stream;
    HR(Instance->CreateStream(&stream));
    HR(stream->InitializeFromFilename(fileName.c_str(), desiredAccess));
    return stream;
  }

  CComPtr<IWICBitmapDecoder> ImagingFactory::CreateDecoder(CComPtr<IWICStream> readStream) const
  {
    CComPtr<IWICBitmapDecoder> decoder;
    HR(Instance->CreateDecoderFromStream(readStream, nullptr, WICDecodeMetadataCacheOnDemand, &decoder));
    return decoder;
  }

  CComPtr<IWICBitmapEncoder> ImagingFactory::CreateEncoder(CComPtr<IWICStream> writeStream, const GUID& format) const
  {
    CComPtr<IWICBitmapEncoder> encoder;
    HR(encoder.CoCreateInstance(format));
    HR(encoder->Initialize(writeStream, WICBitmapEncoderNoCache));
    return encoder;
  }

  CComPtr<IWICBitmapScaler> ImagingFactory::CreateScaler() const
  {
    CComPtr<IWICBitmapScaler> scaler;
    HR(Instance->CreateBitmapScaler(&scaler));
    return scaler;
  }

  Image::Image(CComPtr<IWICStream> source, ImagingFactory::Sptr factory)
  : Source(source)
  , Factory(factory)
  {
  }

  void Image::To(CComPtr<IWICStream> target, const GUID& format) const
  {
    CComPtr<IWICBitmapDecoder> decoder = Factory->CreateDecoder(Source);
    CComPtr<IWICBitmapEncoder> encoder = Factory->CreateEncoder(target, format);
    UINT frameCount = 0;
    HR(decoder->GetFrameCount(&frameCount));

    for (UINT frameIndex = 0; frameIndex < frameCount; ++frameIndex)
    {
      TransformFrame(frameIndex, *decoder, *encoder);
    }
    HR(encoder->Commit());
  }

  void Image::TransformFrame(UINT frameIndex, IWICBitmapDecoder& decoder, IWICBitmapEncoder& encoder) const
  {
    CComPtr<IWICBitmapFrameEncode> targetFrame;
    const HRESULT error = encoder.CreateNewFrame(&targetFrame, 0);
    if (error == WINCODEC_ERR_UNSUPPORTEDOPERATION)
    {
      // TODO: trace
      return;
    }

    CComPtr<IWICBitmapFrameDecode> sourceFrame;
    HR(decoder.GetFrame(frameIndex, &sourceFrame));

    UINT width = 0;
    UINT height = 0;
    HR(sourceFrame->GetSize(&width, &height));
    GUID pixelFormat = {};
    HR(sourceFrame->GetPixelFormat(&pixelFormat));

    HR(targetFrame->Initialize(0));
    HR(targetFrame->SetSize(width, height));
    HR(targetFrame->SetPixelFormat(&pixelFormat));
    HR(targetFrame->WriteSource(sourceFrame, 0));
    HR(targetFrame->Commit());
  }

  void Image::TransformFrame(UINT frameIndex, IWICBitmapDecoder& decoder, IWICBitmapEncoder& encoder, UINT frameWidth, UINT frameHeight, bool fitToFrame) const
  {
    CComPtr<IWICBitmapFrameEncode> targetFrame;
    const HRESULT error = encoder.CreateNewFrame(&targetFrame, 0);
    if (error == WINCODEC_ERR_UNSUPPORTEDOPERATION)
    {
      // TODO: trace
      return;
    }

    CComPtr<IWICBitmapFrameDecode> sourceFrame;
    HR(decoder.GetFrame(frameIndex, &sourceFrame));
    UINT originalWidth = 0;
    UINT originalHeight = 0;
    HR(sourceFrame->GetSize(&originalWidth, &originalHeight));

    UINT targetWidth = frameWidth;
    UINT targetHeight = frameHeight;

    if (fitToFrame)
    {
      const double kx = originalWidth * 1. / frameWidth;
      const double ky = originalHeight * 1. / frameHeight;
      const double scaleIndex = max(kx, ky);
      targetWidth = static_cast<UINT>(originalWidth * 1. / scaleIndex);
      targetHeight = static_cast<UINT>(originalHeight * 1. / scaleIndex);
    }

    CComPtr<IWICBitmapScaler> sourceScaler = Factory->CreateScaler();
    HR(sourceScaler->Initialize(sourceFrame, targetWidth, targetHeight, WICBitmapInterpolationModeLinear));
    GUID pixelFormat = {};
    HR(sourceScaler->GetPixelFormat(&pixelFormat));

    HR(targetFrame->Initialize(0));
    HR(targetFrame->SetPixelFormat(&pixelFormat));
    HR(targetFrame->WriteSource(sourceScaler, 0));
    HR(targetFrame->Commit());
  }

  void Image::ToFrame(CComPtr<IWICStream> target, const GUID& format, UINT frameWidth, UINT frameHeight, bool fitToFrame) const
  {
    CComPtr<IWICBitmapDecoder> decoder = Factory->CreateDecoder(Source);
    CComPtr<IWICBitmapEncoder> encoder = Factory->CreateEncoder(target, format);
    UINT frameCount = 0;
    HR(decoder->GetFrameCount(&frameCount));

    for (UINT frameIndex = 0; frameIndex < frameCount; ++frameIndex)
    {
      TransformFrame(frameIndex, *decoder, *encoder, frameWidth, frameHeight, fitToFrame);
    }
    HR(encoder->Commit());
  }
}
