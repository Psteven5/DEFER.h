#ifndef _DEFER_H_
#define _DEFER_H_

#undef CAT
#undef _CAT
#define _CAT(a, b) a ## b
#define CAT(a, b) _CAT(a, b)

#ifdef __GNUC__
  #undef DEFER_START
  #define DEFER_START(N) \
    void *_DEFER_ENV[N + 1]; \
    int _DEFER_I = 0
  #undef DEFER_END
  #define DEFER_END() do { \
    _DEFER_ENV[0] = &&CAT(_DEFER_END_, __LINE__); \
    goto *_DEFER_ENV[_DEFER_I--]; \
    CAT(_DEFER_END_, __LINE__): \
  } while (0)
  #undef DEFER
  #define DEFER(...) do { \
    _DEFER_ENV[++_DEFER_I] = &&CAT(_DEFER_, __LINE__); \
    if (0) { \
    CAT(_DEFER_, __LINE__): \
      __VA_ARGS__; \
      goto *_DEFER_ENV[_DEFER_I--]; \
    } \
  } while (0)
#elif
  #include <setjmp.h>
  #undef DEFER_START
  #define DEFER_START(N) \
    jmp_buf _DEFER_ENV[N + 1]; \
    int _DEFER_I = 0
  #undef DEFER_END
  #define DEFER_END() do { \
    int CAT(_DEFER_END_, __LINE__) = setjmp(_DEFER_ENV[0]); \
    if (!CAT(_DEFER_END_, __LINE__)) { \
      longjmp(_DEFER_ENV[_DEFER_I--], 1); \
    } \
  } while (0)
  #undef DEFER
  #define DEFER(...) do { \
    int CAT(_DEFER_CODE_, __LINE__) = setjmp(_DEFER_ENV[++_DEFER_I]); \
    if (CAT(_DEFER_CODE_, __LINE__)) { \
      __VA_ARGS__; \
      longjmp(_DEFER_ENV[_DEFER_I--], 1); \
    } \
  } while (0)
#endif

#endif /* _DEFER_H_ */
