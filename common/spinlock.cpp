#include "spinlock.h"

void Spinlock::lock() noexcept {
  for (;;) {
    if (!lock_.exchange(true, std::memory_order_acquire)) {
      return;
    }
  }
}