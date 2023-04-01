#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include "generic.h"
#include "noncopyable.h"

class Buffer final : Noncopyable {
public:
  Buffer(size_t size);
  ~Buffer();
  size_t Append(const void *data, size_t size);
  size_t Append(const char *data, size_t size);

  size_t Capacity() const;
  size_t Writable() const;
  size_t Readable() const;
  char *BeginWrite() const;
  char *BeginRead() const;
  void HasWritten(size_t size);
  void HasRead(size_t size);
  void Retrieve();

private:
  CACHE_PADDING(0);
  volatile size_t read_ = 0;
  CACHE_PADDING(1);
  volatile size_t write_ = 0;
  CACHE_PADDING(2);
  char *array_ = nullptr;
  CACHE_PADDING(3);
  size_t size_ = 0;
  CACHE_PADDING(4);
};

#endif // __BUFFER_H__