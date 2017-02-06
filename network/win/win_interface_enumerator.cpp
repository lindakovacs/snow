#include "win_interface_enumerator.h"

#include <stdexcept>
#include <string>
#include <ws2tcpip.h>

namespace Network
{
  WinInterfaceEnumerator::WinInterfaceEnumerator(const std::wstring& nodeName, const std::wstring& serviceName)
    : AddressEntries(nullptr)
  {
    // prepare the hints for the type of socket we are interested in
    ADDRINFOW hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (::GetAddrInfoW(nodeName.empty() ? nullptr : nodeName.c_str(), serviceName.c_str(), &hints, &AddressEntries))
    {
      throw std::runtime_error("GetAddrInfoW failed with error: " + std::to_string(::WSAGetLastError()));
    }
    if (AddressEntries == nullptr)
    {
      throw std::runtime_error("GetAddrInfoW returned entries = NULL");
    }
  }

  WinInterfaceEnumerator::~WinInterfaceEnumerator()
  {
    ::FreeAddrInfoW(AddressEntries);
  }

  std::vector<AddressInformation> WinInterfaceEnumerator::Gather()
  {
    std::vector<AddressInformation> interfaces;
    for (ADDRINFOW* entry = AddressEntries; entry != nullptr; entry = entry->ai_next)
    {
      AddressInformation info = {};
      info.Family = entry->ai_family;
      info.Socktype = entry->ai_socktype;
      info.Protocol = entry->ai_protocol;
      interfaces.push_back(std::move(info));
    }
    return std::move(interfaces);
  }

  InterfaceEnumerator::Sptr CreateInterfaceEnumerator(const std::wstring& nodeName, const std::wstring& serviceName)
  {
    return InterfaceEnumerator::Sptr(new WinInterfaceEnumerator(nodeName, serviceName));
  }
}
