/*
BSD 3-Clause License

Copyright (c) 2024, Pieter Stevens

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _DEFER_H_
#define _DEFER_H_

/* _DEFER_CAT: concatenate identifiers */
#undef _DEFER_CAT
#undef __DEFER_CAT
#define __DEFER_CAT(a, b) a ## b
#define _DEFER_CAT(a, b) __DEFER_CAT(a, b)

/* DEFER_START(N): Call to create a defer scope with N possible defers */
#undef DEFER_START
#define DEFER_START(N)

/* DEFER_END(): Call to end a defer scope. */
/*   This calls all the defers added between this and DEFER_START (in opposite order of definition). */
/*   After which control flow resumes normally. */
#undef DEFER_END
#define DEFER_END()

/* DEFER(...): Adds "..." as a defer to the current defer context. */
/*   Can only be used between a DEFER_START and DEFER_END. */
/*   "..." will only execute at DEFER_END. */
#undef DEFER
#define DEFER(...)

/* If GNU C supporting compilers are used (GCC, Clang), then use "labels as values" for the implementation. */
#ifdef __GNUC__
  #undef DEFER_START
  #define DEFER_START(N) \
    void *_DEFER_ENV[N + 1]; \
    int _DEFER_I = 0
  
  #undef DEFER_END
  #define DEFER_END() do { \
    _DEFER_ENV[0] = &&_DEFER_CAT(_DEFER_END_, __LINE__); \
    goto *_DEFER_ENV[_DEFER_I--]; \
    _DEFER_CAT(_DEFER_END_, __LINE__): \
  } while (0)
  
  #undef DEFER
  #define DEFER(...) do { \
    _DEFER_ENV[++_DEFER_I] = &&_DEFER_CAT(_DEFER_, __LINE__); \
    if (0) { \
    _DEFER_CAT(_DEFER_, __LINE__): \
      __VA_ARGS__; \
      goto *_DEFER_ENV[_DEFER_I--]; \
    } \
  } while (0)

/* If not GCC (MSVC, TCC, etc.), use setjmp.h */
#elif
  #include <setjmp.h>

  #undef DEFER_START
  #define DEFER_START(N) \
    jmp_buf _DEFER_ENV[N + 1]; \
    int _DEFER_I = 0

  #undef DEFER_END
  #define DEFER_END() do { \
    int _DEFER_CAT(_DEFER_END_, __LINE__) = setjmp(_DEFER_ENV[0]); \
    if (!_DEFER_CAT(_DEFER_END_, __LINE__)) { \
      longjmp(_DEFER_ENV[_DEFER_I--], 1); \
    } \
  } while (0)

  #undef DEFER
  #define DEFER(...) do { \
    int _DEFER_CAT(_DEFER_CODE_, __LINE__) = setjmp(_DEFER_ENV[++_DEFER_I]); \
    if (_DEFER_CAT(_DEFER_CODE_, __LINE__)) { \
      __VA_ARGS__; \
      longjmp(_DEFER_ENV[_DEFER_I--], 1); \
    } \
  } while (0)
#endif

#endif /* _DEFER_H_ */
