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

#ifndef DEFER_H_d41d8cd98f00b204e9800998ecf8427e
#define DEFER_H_d41d8cd98f00b204e9800998ecf8427e

/* DEFER_START(N): Call to create a defer scope with N possible defers. */
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
  
  /* _DEFER_CAT(a, b): Concatenate identifier b to the end of identifier a. */
  #undef DEFER_CAT_d41d8cd98f00b204e9800998ecf8427e
  #undef DEFER_CAT__d41d8cd98f00b204e9800998ecf8427e
  #define DEFER_CAT__d41d8cd98f00b204e9800998ecf8427e(a, b) a ## _d41d8cd98f00b204e9800998ecf8427e_ ## b
  #define DEFER_CAT_d41d8cd98f00b204e9800998ecf8427e(a, b) DEFER_CAT(a, b)

  #undef DEFER_START
  #define DEFER_START(N) \
    void *DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[N + 1]; \
    int DEFER_I_d41d8cd98f00b204e9800998ecf8427e = 0
  
  #undef DEFER_END
  #define DEFER_END() do { \
    DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[0] = &&DEFER_CAT_d41d8cd98f00b204e9800998ecf8427e(DEFER_END, __LINE__); \
    goto *DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[DEFER_I_d41d8cd98f00b204e9800998ecf8427e--]; \
    DEFER_CAT_d41d8cd98f00b204e9800998ecf8427e(DEFER_END, __LINE__): \
  } while (0)
  
  #undef DEFER
  #define DEFER(...) do { \
    DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[++DEFER_I_d41d8cd98f00b204e9800998ecf8427e] = &&DEFER_CAT_d41d8cd98f00b204e9800998ecf8427e(DEFER, __LINE__); \
    if (0) { \
    DEFER_CAT_d41d8cd98f00b204e9800998ecf8427e(DEFER, __LINE__): \
      __VA_ARGS__; \
      goto *DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[DEFER_I_d41d8cd98f00b204e9800998ecf8427e--]; \
    } \
  } while (0)

/* If no GNU C (MSVC, TCC, etc.), use setjmp.h */
#elif
  #include <setjmp.h>

  #undef DEFER_START
  #define DEFER_START(N) \
    jmp_buf DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[N + 1]; \
    int DEFER_I_d41d8cd98f00b204e9800998ecf8427e = 0

  #undef DEFER_END
  #define DEFER_END() do { \
    if (!setjmp(DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[0])) { \
      longjmp(DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[DEFER_I_d41d8cd98f00b204e9800998ecf8427e--], 1); \
    } \
  } while (0)

  #undef DEFER
  #define DEFER(...) do { \
    if (setjmp(DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[++DEFER_I_d41d8cd98f00b204e9800998ecf8427e])) { \
      __VA_ARGS__; \
      longjmp(DEFER_ENV_d41d8cd98f00b204e9800998ecf8427e[DEFER_I_d41d8cd98f00b204e9800998ecf8427e--], 1); \
    } \
  } while (0)
#endif

#endif /* _DEFER_H_ */
