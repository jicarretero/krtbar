#include "defs.h"

/* Define your functions coded in C  -- your modules */
void get_weather(char *output);
void get_pomodoro_status(char *output);
void get_battery_state(char *output);
void get_used_mem(char *output);
void get_date_time(char *output);
void get_builtin_pomodoro_status(char *output);

void toggle_pomodoro();
void toggle_builtin_pomodoro();

#define FUNCTION(f) f, NULL
#define SCRIPT(s) NULL, s
#define NOTHING NULL, NULL

static const char *separator = "| ";

static const component components[] = {
     {SCRIPT("/home/jicg/.local/bin/playing.sh"), 1, FG1, BG1,
      SCRIPT("/usr/bin/playerctl play-pause")},
     {FUNCTION(get_weather), 600, FG1, BG1, NOTHING},
    /*    {FUNCTION(get_pomodoro_status), 1, FG1, BG1,
       FUNCTION(toggle_pomodoro)}, */
    {FUNCTION(get_builtin_pomodoro_status), 1, FG1, BG1,
     FUNCTION(toggle_builtin_pomodoro)},
    {FUNCTION(get_battery_state), 10, FG1, BG1, NOTHING},
    {FUNCTION(get_used_mem), 5, FG1, BG1, NOTHING},
    {SCRIPT("/home/jicg/.local/bin/qsound.sh"), 1, FG1, BG1,
     SCRIPT("/usr/bin/pavucontrol")},
    {FUNCTION(get_date_time), 10, FG1, BG1, SCRIPT("/usr/bin/gsimplecal")},
};

/* static const component components[] = {
    {get_weather, NULL, 600, FG1, BG1, NULL},
    {get_pomodoro_status, NULL, 1, FG1, BG1,
     "/usr/bin/curl -X POST localhost:4884/pomodoro/toggle"},
    {get_battery_state, NULL, 10, FG1, BG1, NULL},
    {get_used_mem, NULL, 5, FG1, BG1, NULL},
    {NULL, "/home/jicg/bin/qsound.sh", 1, FG1, BG1, "/usr/bin/pavucontrol"},
    {get_date_time, NULL, 10, FG1, BG1, "/usr/bin/gsimplecal"}}; */
