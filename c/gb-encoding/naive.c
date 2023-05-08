#include "naive.h"
#include <iconv.h>

bool naive_is_gb(char *data, size_t len) {
  size_t utf8_len = len * 2;
  char *utf8_str = malloc(utf8_len);

  iconv_t cd = iconv_open("UTF-8", "GB18030");

  char *inbuf = data;
  char *outbuf = utf8_str;
  size_t inbytesleft = len;
  size_t outbytesleft = utf8_len;

  iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);

  free(utf8_str);
  iconv_close(cd);
  return inbytesleft == 0;
}
