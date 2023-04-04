#include "../defs.h"
#include "../util.h"

typedef __uintmax_t uintmax_t;

uintmax_t read_memory() {
  uintmax_t memfree = 0;
  uintmax_t memtotal = 0;
  uintmax_t memavailable = 0;
  uintmax_t buffers = 0, cached = 0;

  int n;

  n = pscanf("/proc/meminfo",
             "MemTotal: %ju kB\n"
             "MemFree: %ju kB\n"
             "MemAvailable: %ju kB\n"
             "Buffers: %ju kB\n"
             "Cached: %ju kB\n",
             &memtotal, &memfree, &memavailable, &buffers, &cached);

  uintmax_t used = (memtotal - memfree - buffers - cached) / 1024;
  return used;
}

void get_used_mem(char *buffer) {
  snprintf(buffer, MAX_COMPONENT_BUFFER, " %lu Mb", read_memory());
}
