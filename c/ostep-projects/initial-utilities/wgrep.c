#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    return EXIT_FAILURE;
  }
  const char *search = argv[1];
  if (argc == 2) {
    // read from std
    char line[1024];
    while (fgets(line, sizeof(line), stdin) != NULL) {
      strchr(line, '\n');
      if (strstr(line, search) != NULL) {
        printf("%s", line);
      }
    }
  } else {
    // read from files
    char *line;
    for (int i = 2; i < argc; i++) {
      FILE *fp = fopen(argv[i], "r");
      if (fp == NULL) {
        printf("wgrep: cannot open file\n");
        return EXIT_FAILURE;
      }
      ssize_t read;
      size_t len = 0;
      while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, search) != NULL) {
          printf("%s", line);
        }
      }
      fclose(fp);
    }
  }

  return EXIT_SUCCESS;
}
