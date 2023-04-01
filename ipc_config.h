#ifndef __IPCCONFIG_H
#define __IPCCONFIG_H

#include <string>

enum IpcStrategy { kShm = 0x01, kTcp = 0x02, kUdp = 0x04 };

struct IpcConfig final {
  std::string name = "ipc";
  std::string shm_name = "nullmax.auto_driving.shared_memory";
  std::string ip_address = "10.211.55.2";
  std::string netmask = "255.255.255.0";
  uint32_t domain = 0;
  uint32_t shm_size = 16 * 1024 * 1024;
  uint16_t thread_pool_size = 2;
  uint32_t heartbeat_time = 5;
  uint32_t heartbeat_expire_time = 15;
  uint32_t strategy = IpcStrategy::kShm | IpcStrategy::kTcp | IpcStrategy::kUdp;
};

#endif