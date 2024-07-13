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

int main(void) {
  foo();
  bar();
}
