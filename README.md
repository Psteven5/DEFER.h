# DEFER.h
DEFER macros in C

DEFER.h defines 3 macros to use:
 * DEFER_START(N): Creates a defer scope with N possible defers.
 * DEFER_END(): Used to end a defer scope (before each return or exit), as this calls all the defers added since DEFER_START.
 * DEFER(...): Defers the code inside the brackets to DEFER_END.

```c
DEFER_START(2);

int x = 1;
if (x) {
  DEFER_START(2);

  DEFER(fputc('\n', stdout));
  DEFER(printf("%i", x));
  ++x;

  DEFER_END();
}

DEFER(printf("world!\n"));
DEFER(printf("Hello, "));

DEFER_END();
```

```
2
Hello, world!
```

The implementation uses an array of jmp_bufs (or addresses of labels in the case of GNU C supporting compilers) to allow unwinding of the defers accumulated and then proceed with control flow like normal.
