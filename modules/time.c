#include "../defs.h"
#include <time.h>

static const char *time_format = " %d-%m-%Y %H:%M";
/* static const char *time_format = " %d-%m-%Y %H:%M:%S"; */

void get_date_time(char *buffer) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);

  size_t ret = strftime(buffer, MAX_COMPONENT_BUFFER, time_format, tm);
}
