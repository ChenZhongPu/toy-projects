#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (!file) {
      printf("wcat: cannot open file\n");
      return EXIT_FAILURE;
    }
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
      printf("%s", buffer);
    }
    fclose(file);
  }

  return EXIT_SUCCESS;
}
