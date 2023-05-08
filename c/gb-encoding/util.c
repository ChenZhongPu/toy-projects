#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *read_file(const char *name, size_t *len) {
  FILE *file = fopen(name, "rb");
  if (file == 0) {
    printf("failed to open file");
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buf = malloc(size);
  fread(buf, size, 1, file);
  fclose(file);
  *len = size;
  return buf;
}

double time_it(bool (*func)(char *, size_t), char *data, size_t len,
               bool *result) {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);
  *result = func(data, len);
  clock_gettime(CLOCK_MONOTONIC, &end);
  double diff = (end.tv_sec - start.tv_sec) * 1e9;
  diff += (end.tv_nsec - start.tv_nsec);
  return diff;
}
