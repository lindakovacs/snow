#pragma once

#include <windows.h>

namespace Core
{
  struct HandleCloser
  {
    void operator () (HANDLE handle) const
    {
      if (handle != NULL && handle != INVALID_HANDLE_VALUE)
      {
        ::CloseHandle(handle);
      }
    }
  };

  struct IconDestroyer
  {
    void operator () (HICON icon) const
    {
      if (icon != NULL)
      {
        ::DestroyIcon(icon);
      }
    }
  };

  struct DcReleaser
  {
    HWND hwnd;

    DcReleaser(HWND theHwnd = NULL)
      : hwnd(theHwnd)
    {
    }

    void operator () (HDC hdc) const
    {
      if (hdc != NULL)
      {
        ::ReleaseDC(hwnd, hdc);
      }
    }
  };

  template <typename Obj = HGDIOBJ>
  struct GdiObjDeleter
  {
    void operator () (Obj obj) const
    {
      if (obj != NULL)
      {
        ::DeleteObject((HGDIOBJ)obj);
      }
    }
  };
}
