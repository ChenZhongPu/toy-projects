#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2000000

/**
 * @brief Encode ints using varint
 *
 * @param buf
 * @param vals
 * @param n
 * @return number of buf used
 */
size_t ints_encode(uint8_t *buf, uint32_t *vals, size_t n) {
  size_t count = 0;
  for (int i = 0; i < n; i++) {
    uint32_t val = vals[i];
    while (val > 0x7f) {
      buf[count++] = (val & 0x7f) | 0x80;
      val >>= 7;
    }
    buf[count++] = val;
  }
  return count;
}

uint32_t *ints_decode(uint8_t *buf, size_t len, size_t *num_values) {
  uint32_t *values = malloc(sizeof(uint32_t) * len);
  size_t value_count = 0;
  int i = 0;
  while (i < len) {
    uint32_t val = 0;
    int shift = 0;
    while (buf[i] & 0x80) {
      val |= (buf[i] & 0x7f) << shift;
      i++;
      shift += 7;
    }
    val |= buf[i] << shift;
    i++;
    values[value_count++] = val;
  }
  *num_values = value_count;
  return values;
}

int main(int argc, char *argv[]) {
  // 1M random integers
  uint32_t vals[N];
  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    vals[i] = rand();
  }
  // encode
  size_t n = sizeof(vals) / sizeof(uint32_t);
  uint8_t *buf = malloc(sizeof(uint8_t) * 4 * n);
  size_t len = ints_encode(buf, vals, n);
  printf("buf len: %zu\n", len);
  struct timespec start, end;

  size_t count;
  clock_gettime(CLOCK_MONOTONIC, &start);
  uint32_t *values = ints_decode(buf, len, &count);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("integer count: %zu\n", count);
  double diff = (end.tv_sec - start.tv_sec) * 1e9;
  diff += (end.tv_nsec - start.tv_nsec);

  printf("deconding time is %f ns\n", diff);

  printf("Throughput: %0.f MInt/s\n", (len / diff * 1e9) / 1e6);
  free(values);
  free(buf);
  return EXIT_SUCCESS;
}
