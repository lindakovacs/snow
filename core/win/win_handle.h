#pragma once

#include "win_handle_detail.h"
#include <memory>

namespace Core
{
  template <typename UniqueHandle>
  class ScopedHandle
  {
  public:
    explicit ScopedHandle(UniqueHandle handle): Handle(std::move(handle)) {}

    operator typename UniqueHandle::element_type() const
    {
      return Handle.get();
    }

  private:
    UniqueHandle Handle;
  };

  typedef std::unique_ptr<std::remove_pointer<HANDLE>::type, HandleCloser> HandleUptr;
  typedef std::unique_ptr<std::remove_pointer<HICON>::type, IconDestroyer> HiconUptr;
  typedef std::unique_ptr<std::remove_pointer<HBITMAP>::type, GdiObjDeleter<HBITMAP>> HbitmapUptr;
  typedef std::unique_ptr<std::remove_pointer<HDC>::type, DcReleaser> HdcUptr;

  typedef std::shared_ptr<ScopedHandle<HandleUptr>> HandleSptr;
  typedef std::shared_ptr<ScopedHandle<HiconUptr>> HiconSptr;
  typedef std::shared_ptr<ScopedHandle<HbitmapUptr>> HbitmapSptr;
  typedef std::shared_ptr<ScopedHandle<HdcUptr>> HdcSptr;
}
