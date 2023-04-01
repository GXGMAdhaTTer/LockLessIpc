#ifndef __IPC_H
#define __IPC_H

#include <functional>
#include <map>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>

#include "ipc_config.h"

class IpcImpl;

class Ipc final {
public:
  using OnMessageFn = std::function<void(const std::string &)>;

  Ipc(const Ipc &) = delete;
  Ipc &operator=(const Ipc &) = delete;
  Ipc(Ipc &&) = delete;
  Ipc &operator=(Ipc &&) = delete;

  bool Publish(const std::string &topic, const std::string &message);
  bool Advertise(const std::set<std::string> &topic);
  bool Subscribe(const std::string &topic, const OnMessageFn &on_message);
  bool Subscribe(const std::map<std::string, OnMessageFn> &subscribe_map);

private:
  std::shared_ptr<IpcImpl> ipc_impl_;
};

#endif