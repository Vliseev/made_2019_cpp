#pragma once
#if defined(__GNUC__)
  #ifndef EXPECT
    #define EXPECT(x, val) __builtin_expect(x, val)
  #endif

  #ifndef LIKELY
    #define LIKELY(x) EXPECT(x, 1)
  #endif
  #ifndef UNLIKELY
    #define UNLIKELY(x) EXPECT(x, 0)
  #endif
#else
  #define LIKELY(x) (x)
  #define UNLIKELY(x) (x)
#endif