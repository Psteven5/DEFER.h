#include <stdio.h>
#include <stdlib.h>

#include "DEFER.h"

int foo(void) {
  
  /* Indicate that we’re going to use 2 defers. */
  DEFER_START(2);
  
  DEFER(printf(" world!\n"));
  DEFER(printf("Hello,"));

  printf("My message: ");
  
  /* The defers execute in reversed order, and only at DEFER_END. */
  /* => My message: Hello, world! */
  DEFER_END();
  
  return 0;
}

int bar(void) {
  DEFER_START(3);
  
  FILE *f = fopen("example.txt", "r");
  if (NULL == f) {
    DEFER_END();
    return -1;
  }
  
  /* References to variables refer to their value at the point of a call to DEFER_END, not at the current point. */
  DEFER(fclose(f));

  int size;
  if (1 != fscanf(f, "%i", &size)) {
    DEFER_END();
    return -2;
  }

  int *nums = malloc(size * sizeof(int));
  if (NULL == nums) {
    DEFER_END();
    return -3;
  }
  DEFER(free(nums));

  for (int i = 0; i < size; ++i) {
    int num;
    if (1 != fscanf(f, "%i", &num)) {
      DEFER_END();
      return -4;
    }
    nums[i] = num;
  }

  DEFER(fputc('\n', stdout));
  for (int i = 0; i < size; ++i) {
    printf("%i ", nums[i]);
  }
  
  DEFER_END();
  return 0;
}

int foobar(void) {
  DEFER_START(2);

  /* Nested defer scopes can be created within C scopes. */
  /* DEFER calls and DEFER_END within this scope will refer to this defer scope. */
  int x = 1;
  if (x) {
    DEFER_START(2);

    DEFER(fputc('\n', stdout));
    DEFER(printf("%i", x));

    DEFER_END();
  }

  DEFER(printf("world!\n"));
  DEFER(printf("Hello, "));

  /* => 1 */
  /*    Hello, world! */
  DEFER_END();
  return 0;
}

int barfoo(void) {
  DEFER_START(4);

  /* ...But defer can also be kept on a function basis, like Go. */
  int x = 1;
  if (x) {
    DEFER(fputc('\n', stdout));
    DEFER(printf("%i", x));
  }

  DEFER(printf("world!\n"));
  DEFER(printf("Hello, "));

  /* => Hello, world! */
  /*    1 */
  DEFER_END();
  return 0;
}

int main(void) {
  foo();
  bar();
  foobar();
  barfoo();
}
