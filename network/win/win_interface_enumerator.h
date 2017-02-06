
#include <network/interface_enumerator.h>
#include <winsock2.h>

namespace Network
{
  class WinInterfaceEnumerator : public InterfaceEnumerator
  {
  public:
    WinInterfaceEnumerator(const std::wstring& nodeName, const std::wstring& serviceName);
    virtual ~WinInterfaceEnumerator();
    virtual std::vector<AddressInformation> Gather();

  private:
    ADDRINFOW* AddressEntries;
  };
}
