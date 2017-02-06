#include "stream_server.h"

#include <network/interface_enumerator.h>
#include <network/resource_initialization.h>

namespace Research
{
  void TestStreamServer()
  {
    Network::ResourceInitialization::Sptr resource = Network::CreateResourceInitialization();
    Network::InterfaceEnumerator::Sptr walker = Network::CreateInterfaceEnumerator(L"localhost", L"12410");
    const std::vector<Network::AddressInformation> interfaces = walker->Gather();
    int k = 0;
  }
}
