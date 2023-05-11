#include "gb18030.h"
#include "naive.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }
  size_t len;
  char *data = read_file(argv[1], &len);
  printf("len is: %zu\n", len);

  bool result;
  double naive_time = time_it(naive_is_gb, data, len, &result);
  printf("naive_is_gb result: %s\n", result ? "true" : "false");
  printf("naive_is_gb time: %f\n", naive_time);

  double fsm_time = time_it(check_by_fsm, data, len, &result);
  printf("check_by_fsm result: %s\n", result ? "true" : "false");
  printf("check_by_fsm time: %f\n", fsm_time);
  printf("speedup: %f\n", naive_time / fsm_time);

  double branch_time = time_it(check_by_range, data, len, &result);
  printf("check_by_range result: %s\n", result ? "true" : "false");
  printf("check_by_range time: %f\n", branch_time);
  printf("speedup: %f\n", naive_time / branch_time);

  return EXIT_SUCCESS;
}
