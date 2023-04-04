#include "defs.h"

/* Define your functions coded in C  -- your modules */
void get_weather(char *output);
void get_pomodoro_status(char *output);
void get_battery_state(char *output);
void get_used_mem(char *output);
void get_date_time(char *output);

void toggle_pomodoro();

#define FI(f) f, NULL
#define SE(s) NULL, s

static const component components[] = {
    {FI(get_weather), 600, FG1, BG1, FI(NULL)},
    {FI(get_pomodoro_status), 1, FG2, BG2, FI(toggle_pomodoro)},
    {FI(get_battery_state), 10, FG1, BG1, SE(NULL)},
    {FI(get_used_mem), 5, FG2, BG2, SE(NULL)},
    {SE("/home/jicg/bin/qsound.sh"), 1, FG1, BG1, SE("/usr/bin/pavucontrol")},
    {FI(get_date_time), 10, FG2, BG2, SE("/usr/bin/gsimplecal")}};

/* static const component components[] = {
    {get_weather, NULL, 600, FG1, BG1, NULL},
    {get_pomodoro_status, NULL, 1, FG2, BG2,
     "/usr/bin/curl -X POST localhost:4884/pomodoro/toggle"},
    {get_battery_state, NULL, 10, FG1, BG1, NULL},
    {get_used_mem, NULL, 5, FG2, BG2, NULL},
    {NULL, "/home/jicg/bin/qsound.sh", 1, FG1, BG1, "/usr/bin/pavucontrol"},
    {get_date_time, NULL, 10, FG2, BG2, "/usr/bin/gsimplecal"}}; */
