#ifndef __IPC_COMMON_H
#define __IPC_COMMON_H

#include <stdint.h>
#include <stddef.h>

static constexpr uint64_t kNanoSecondPerSeconds = 1000000000;

static constexpr size_t kThreadPoolCount = 2;
static constexpr size_t kThreadPoolQueueSize = 16*1024;

static constexpr size_t kBufferSize = 1*1024*1024;
static constexpr size_t kSocketBuffre = 128*1024;
static constexpr size_t kTcpRecvBufferSize = 128*1024;
static constexpr size_t kTcpSendBufferSize = 128*1024;
static constexpr size_t kUdpRecvBufferSize = 128*1024;

static constexpr size_t kMagicNumber = 0xDEADBEEF;

enum Module : uint32_t {
  PUBLISHER = 0x1,
  SUBSCRIBER = 0x2
};

#endif