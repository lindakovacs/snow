#pragma once

#include <cstdint>

namespace Network
{
  // This is the default size of the data buffer that'll be used in recv each time. In real apps, this value will be much higher.
  const uint32_t RecvDataSize = 4096;

  // This structure defines the contents of the data buffer that'll be used to store the messages that are received from and sent to the client.
  struct Buffer
  {
    char Buf[RecvDataSize]; // the data buffer
    uint32_t DataSize; // length of the actual data present
    uint32_t SendOffset; // position of the next byte in buf to be sent
    bool IsNewData; // true if buf has not been fully sent
  };
}
