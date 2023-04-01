#ifndef __GENERIC_H__
#define __GENERIC_H__

#ifndef CACHELINE_SIZE
#define CACHELINE_SIZE 64
#endif

#define COMBINE(x, y) x##y
#define CACHE_PADDING(N) char COMBINE(__cache_padding, N)[CACHELINE_SIZE]

#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#ifndef __attribute_used
#define __attribute_used __attribute__((__used__))
#endif
#ifndef __attribute_unused
#define __attribute_unused __attribute__((__unused__))
#endif
#define __packed __attribute__((__packed__))

#define Memcpy memcpy

#endif // __GENERIC_H__