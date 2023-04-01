#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>
#include <stddef.h>

struct MessageHeader {
  static constexpr uint32_t kCrc = 0xDEADBEEF;

  uint32_t topic_len;
  uint32_t message_len;
  uint32_t crc = kCrc;

  inline static constexpr size_t Size() {
    return sizeof(MessageHeader);
  }
};

#endif // __PROTOCOL_H__