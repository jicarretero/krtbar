#ifndef __CONFIG_H__
#define __CONFIG_H__
#include "colors.h"
#include <stdio.h>
#include <sys/types.h>

/* Define functions coded in C */
void get_weather(char *output);
void get_pomodoro_status(char *output);
void get_battery_state(char *output);
void get_used_mem(char *output);
void get_date_time(char *output);

typedef enum { FUNCTION = 0, STRING = 1 } component_type;

#define COMPONENT_LIMIT (char)31
#define MAX_COMPONENT_BUFFER 2048

typedef struct component {
  void (*fn)(char *);
  const char *s;
  const char *fg_color;
  const char *bg_color;
  const char *on_click;
} component;

typedef struct component_control {
  pid_t pid;
  char buffer[MAX_COMPONENT_BUFFER];
  const component *p_component;
} component_control;

static const component components[] = {
    {get_weather, NULL, FG1, BG1, NULL},
    {get_pomodoro_status, NULL, FG2, BG2,
     "/usr/bin/curl -X POST localhost:4884/pomodoro/toggle"},
    {get_battery_state, NULL, FG1, BG1, NULL},
    {get_used_mem, NULL, FG2, BG2, NULL},
    {NULL, "/home/jicg/bin/qsound.sh", FG1, BG1, "/usr/bin/pavucontrol"},
    {get_date_time, NULL, FG2, BG2, "/usr/bin/gsimplecal"}};

#endif
