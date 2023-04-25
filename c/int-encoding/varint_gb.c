#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 2000000
/**
 * @brief encode an array of int32 using varint-gb
 *
 * @param buf
 * @param vals
 * @param n
 * @return size of buf used
 */
size_t encode_int32(uint8_t *buf, uint32_t *vals, size_t n) {
  size_t buf_idx = 0;
  for (int i = 0; i < n; i += 4) {
    // tags
    size_t tag_idx = buf_idx;
    buf[buf_idx++] = 0x00;
    size_t group = (i + 3 < n) ? 4 : (n - i);
    for (int j = 0; j < group; j++) {
      uint32_t val = vals[i + j];
      size_t len = 0;
      while (val > 0xff) {
        buf[buf_idx++] = val & 0xff;
        val >>= 8;
        len++;
      }
      buf[buf_idx++] = val;
      len++;
      buf[tag_idx] |= (len - 1) << (6 - 2 * j);
    }
  }
  return buf_idx;
}

uint32_t *decode_int32(uint8_t *buf, const size_t buf_len, size_t *count) {
  uint32_t *vals = (uint32_t *)malloc(sizeof(uint32_t) * buf_len);
  size_t num_count = 0;
  size_t i = 0;
  uint8_t masks[] = {0xC0, 0x30, 0x0C, 0x03};
  while (i < buf_len) {
    uint8_t tag = buf[i];
    i++;
    for (int j = 0; j < 4; j++) {
      size_t len = ((tag & masks[j]) >> (6 - 2 * j)) + 1;
      uint32_t val = 0;
      for (size_t k = 0; k < len; k++) {
        val |= buf[i + k] << (8 * k);
      }
      vals[num_count++] = val;
      i += len;
      if (i >= buf_len) {
        break;
      }
    }
  }
  *count = num_count;
  return vals;
}

int main(int argc, char *argv[]) {
  // 1M random integers
  uint32_t vals[N];

  srand(time(NULL));
  for (int i = 0; i < N; i++) {
    vals[i] = rand();
  }
  size_t n = sizeof(vals) / sizeof(uint32_t);
  uint8_t *buf = malloc(sizeof(uint8_t) * 4 * n);
  size_t len = encode_int32(buf, vals, n);
  printf("buf len: %zu\n", len);

  struct timespec start, end;
  size_t count;
  clock_gettime(CLOCK_MONOTONIC, &start);
  uint32_t *result = decode_int32(buf, len, &count);
  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("integer count: %zu\n", count);

  double diff = (end.tv_sec - start.tv_sec) * 1e9;
  diff += (end.tv_nsec - start.tv_nsec);
  printf("deconding time using varint-gb is %f ns\n", diff);
  printf("Throughput: %0.f MInt/s\n", (len / diff * 1e9) / 1e6);
  free(result);
  free(buf);
  return EXIT_SUCCESS;
}
