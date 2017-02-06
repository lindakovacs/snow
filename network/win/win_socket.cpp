#include "win_socket.h"

#include <core/string_utils.h>

#include <cstdint>
#include <stdexcept>
#include <string>

namespace Network
{
  // This is the number of clients that can be accomodated in the FD_SETs that
  // are used in the select command. In spite of the name referring to the
  // max number of clients, the actual number of clients that will be
  // accomodated will be a little less because the select will also include the
  // server interfaces on which the app is listening.
  // If you want to increase this value, first undefine FD_SETSIZE and redefine
  // FD_SETSIZE to the required value before including winsock2.h
  const std::int32_t MaxClients = FD_SETSIZE;

  WinSocket::WinSocket(SOCKET handle)
    : Handle(handle)
  {
  }

  WinSocket::~WinSocket()
  {
    if (Handle != INVALID_SOCKET)
    {
      ::closesocket(Handle);
    }
  }

  void WinSocket::EnableNonBlockingMode()
  {
    // for non-blocking select, we need to explicitly make the socket non-blocking by this call, whereas for WSAAsyncSelect this is not required as WSAAsyncSelect itself makes the socket non-blocking.
    unsigned long nonBlocking = 1;
    if (::ioctlsocket(Handle, FIONBIO, &nonBlocking) == SOCKET_ERROR)
    {
      throw std::runtime_error(Core::Format("can't put socket into non-blocking mode with error = %d\n", ::WSAGetLastError()));
    }
  }

  std::string MakeErrorMessage(const std::string& whatFailed, const AddressInformation& address)
  {
    const std::string& message = whatFailed + " failed, error = %d, ignoring this address (family = %d, socktype = %d, protocol = %d) and continuing with the next.\n";
    return Core::Format(message.c_str(), ::WSAGetLastError(), address.Family, address.Socktype, address.Protocol);
  }

  std::vector<Socket::Sptr> CreateListeningSockets(const std::vector<AddressInformation>& interfaces)
  {
    std::vector<Socket::Sptr> sockets;
    for (const AddressInformation& address : interfaces)
    {
      const SOCKET handle = ::WSASocket(address.Family, address.Socktype, address.Protocol, nullptr, 0, 0);
      if (handle == INVALID_SOCKET)
      {
        // TODO: trace MakeErrorMessage("WSASocket", address);
        continue;
      }
      Socket::Sptr socket = std::make_shared<WinSocket>(handle);

      if (::bind(handle, static_cast<SOCKADDR*>(address.Address), address.AddressSize) != NO_ERROR)
      {
        // TODO: trace MakeErrorMessage("bind", address);
        continue;
      }
      // TODO: trace Socket bound successfully
      if (::listen(handle, MaxClients) != NO_ERROR)
      {
        // TODO: trace MakeErrorMessage("bind", address);
        continue;
      }

      socket->EnableNonBlockingMode();

      // TODO: trace    all went well. add this to the list of listening sockets.
      sockets.push_back(socket);
    }
    return std::move(sockets);
  }
}
