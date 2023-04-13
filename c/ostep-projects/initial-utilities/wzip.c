#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wzip: file1 [file2 ...]\n");
    return EXIT_FAILURE;
  }

  char start;
  int count;
  for (int i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    if (i == 1) {
      // first file
      start = fgetc(fp);
      count = 1;
    }
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
      if (ch == start) {
        count++;
      } else {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&start, sizeof(char), 1, stdout);
        start = ch;
        count = 1;
      }
    }
    fclose(fp);
  }
  fwrite(&count, sizeof(int), 1, stdout);
  fwrite(&start, sizeof(char), 1, stdout);

  return EXIT_SUCCESS;
}
