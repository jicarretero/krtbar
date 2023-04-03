#include <stdarg.h>
#include <stdio.h>

FILE *flog = NULL;

void klog(char *fmt, ...) {
  va_list ap;
  if (flog == NULL) {
    flog = fopen("/tmp/dwm.klog", "w");
  }
  va_start(ap, fmt);
  vfprintf(flog, fmt, ap);
  va_end(ap);
  fprintf(flog, "\n");
  fflush(flog);
}
