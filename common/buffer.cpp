#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include "atomic_64.h"
#include "buffer.h"
#include "generic.h"
#include "logger.h"
#include "noncopyable.h"

Buffer::Buffer(size_t size) : size_(size) {
  if (posix_memalign((void **)&array_, CACHELINE_SIZE, size) != 0) {
    NM_ERROR("alloc memory {} error", size);
  }
}

Buffer::~Buffer() {
  if (array_) {
    free(array_);
    array_ = nullptr;
  }
}

size_t Buffer::Append(const void *data, size_t size) {
  return Append(static_cast<const char *>(data), size);
}

size_t Buffer::Append(const char *data, size_t size) {
  const int need = size - Writable();
  if (unlikely(need > 0)) {
    array_ = static_cast<char *>(std::realloc(array_, this->size_ + need));
    assert(array_);
    this->size_ += need;
  }
  std::memcpy(array_ + write_, data, size);
  write_ += size;
  return size;
}

size_t Buffer::Capacity() const {
  return size_;
}

size_t Buffer::Writable() const {
  return size_ - write_;
}

size_t Buffer::Readable() const {
  return write_ - read_;
}

char* Buffer::BeginWrite() const {
  return array_ + write_;
}

char* Buffer::BeginRead() const {
  return array_ + read_;
}

void Buffer::HasWritten(size_t size) {
  write_ += size;
}

void Buffer::HasRead(size_t size) {
  read_ += size;
}

void Buffer::Retrieve() {
  size_t len = Readable();
  if (read_) {
    std::memmove(array_, BeginRead(), len);
  }
  read_ = 0;
  write_ = len;
}

