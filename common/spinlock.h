#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#include <atomic>
#include "atomic_64.h"

class Spinlock final {
public:
  void lock() noexcept;
  bool try_lock() noexcept;
  void unlock() noexcept;

private:
  std::atomic<bool> lock_ = {0};
};

template <typename T>
class SpinlockGuard final {
public:
  SpinlockGuard(T& lock) noexcept;
  ~SpinlockGuard() noexcept;

private:
  T& lock_;
};

#endif // __SPINLOCK_H__