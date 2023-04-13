#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wunzip: file1 [file2 ...]\n");
    return EXIT_FAILURE;
  }
  for (int i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    int count;
    char c;
    while (fread(&count, sizeof(int), 1, fp) == 1) {
      fread(&c, sizeof(char), 1, fp);
      for (int j = 0; j < count; j++) {
        printf("%c", c);
      }
    }
    fclose(fp);
  }
  return EXIT_SUCCESS;
}
