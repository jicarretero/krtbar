#include "util.h"
#include <errno.h>
#include <stdio.h>

extern int errno;

int pscanf(const char *path, const char *fmt, ...) {
  FILE *fp = NULL;
  va_list ap;
  int n;

  fp = fopen(path, "r");

  if (fp == NULL) {
    fprintf(stderr, "ERROR opening file %s - error=%d\n", path, errno);
    return -1;
  }
  va_start(ap, fmt);
  n = vfscanf(fp, fmt, ap);
  va_end(ap);
  fclose(fp);

  return (n == EOF) ? -1 : 0;
}
