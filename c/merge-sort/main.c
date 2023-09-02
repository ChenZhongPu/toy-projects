#include "merge.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long time_it(void (*func)(int *, int), int *data, int len) {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  func(data, len);
  clock_gettime(CLOCK_MONOTONIC, &end);
  long diff = (end.tv_sec - start.tv_sec) * 1e9;
  diff += (end.tv_nsec - start.tv_nsec);
  return diff;
}

void print_array(int *a, int len) {
  for (int i = 0; i < len; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
}

bool is_sorted(int *a, int len) {
  for (int i = 1; i < len; i++) {
    if (a[i] < a[i - 1]) {
      return false;
    }
  }
  return true;
}

int main(void) {
  srand(time(NULL)); // seed random number generator
  const int len = 10000000;
  int *a = malloc(len * sizeof(int));
  int *b = malloc(len * sizeof(int));
  for (int i = 0; i < len; i++) {
    a[i] = b[i] = rand();
  }

  long t1 = time_it(sort1, a, len);
  printf("sort1 took %ld ns\n", t1);
  printf("is sorted: %s\n", is_sorted(a, len) ? "true" : "false");

  /* long t2 = time_it(sort2, b, len); */
  /* printf("sort2 took %ld ns\n", t2); */
  /* printf("is sorted: %s\n", is_sorted(b, len) ? "true" : "false"); */

  /* long t3 = time_it(sort3, b, len); */
  /* printf("sort3 took %ld ns\n", t3); */
  /* printf("is sorted: %s\n", is_sorted(b, len) ? "true" : "false"); */

  long t4 = time_it(sort4, b, len);
  printf("sort4 took %ld ns\n", t4);
  printf("is sorted: %s\n", is_sorted(b, len) ? "true" : "false");

  return EXIT_SUCCESS;
}
