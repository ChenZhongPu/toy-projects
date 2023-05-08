#include <stdbool.h>
#include <stdlib.h>

char *read_file(const char *name, size_t *len);

double time_it(bool (*func)(char *, size_t), char *data, size_t len,
               bool *result);
