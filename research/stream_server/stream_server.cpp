#include "stream_server.h"

#include <network/interface_enumerator.h>
#include <network/resource_initialization.h>
#include <network/socket.h>

namespace Research
{
  void TestStreamServer()
  {
    Network::ResourceInitialization::Sptr resource = Network::CreateResourceInitialization();
    Network::InterfaceEnumerator::Sptr enumerator = Network::CreateInterfaceEnumerator(L"localhost", L"12410");
    const std::vector<Network::AddressInformation> interfaces = enumerator->Gather();
    std::vector<Network::Socket::Sptr> listeningSockets = Network::CreateListeningSockets(interfaces);

    int k = 0;
  }
}
