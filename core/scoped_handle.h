#pragma once

namespace Core
{
  template <typename T>
  class ScopedHandle
  {
  public:
    typedef void(*F)(T);
    ScopedHandle(T handle, F deallocator)
    : Handle(handle)
    , Deallocator(deallocator)
    {
    }

    ~ScopedHandle()
    {
      if (Handle)
      {
        Deallocator(Handle);
      }
    }

    ScopedHandle(const ScopedHandle&) = delete;
    ScopedHandle& operator = (const ScopedHandle&) = delete;

    operator T() const
    {
      return Handle;
    }

  private:
    const T Handle;
    const F Deallocator;
  };
}
