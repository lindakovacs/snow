#pragma once

#include <memory>
#include <vector>

namespace Network
{
  struct AddressInformation
  {
    int Family = 0;
    int Socktype = 0;
    int Protocol = 0;
  };

  class InterfaceEnumerator
  {
  public:
    typedef std::shared_ptr<InterfaceEnumerator> Sptr;
    virtual ~InterfaceEnumerator() {}
    virtual std::vector<AddressInformation> Gather() = 0;
  };

  InterfaceEnumerator::Sptr CreateInterfaceEnumerator(const std::wstring& nodeName, const std::wstring& serviceName);
}
