#ifndef __DEFS_H__
#define __DEFS_H__
#include "colors.h"
#include <stdio.h>
#include <sys/types.h>

typedef enum { FUNCTION = 0, STRING = 1 } component_type;

#define COMPONENT_LIMIT (char)31
#define MAX_COMPONENT_BUFFER 2048

typedef struct component {
  void (*fn)(char *);
  const char *s;
  const u_int64_t upd_time;
  const char *fg_color;
  const char *bg_color;
  void (*fn_click)();
  const char *on_click;
} component;

typedef struct component_control {
  pid_t pid;
  char buffer[MAX_COMPONENT_BUFFER];
  const component *p_component;
} component_control;

#endif
