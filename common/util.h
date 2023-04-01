#ifndef __UTIL_H__
#define __UTIL_H__

#include <iostream>

static inline void HexDump(uint8_t* data, size_t size) {
  size_t i, j;

  /* Dump whole data */
  for (i = 0; i < size; ++i) {
    if (i && i % 16 == 0) {
      printf("        ");
      for (j = i - 16; j < i; j++) {
        if (data[j] >= 32 && data[j] <=128) {
          printf("%c", (uint8_t)data[j]);
        } else {
          printf(".");
        }
      }
      printf("\n");
    }
  }
}

#endif // __UTIL_H__