#include "../util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define pomodoro_color "^c#30f0ff^"
#define pomodoro_paused "^c#ff0000^"
#define pomodoro_break "^c#ffff00^"

typedef enum { POMODORO = 0, BREAK = 1, LONG_BREAK = 2 } kindof_timer;
typedef enum { STOPPED = 0, STARTED = 1 } pomodorostatus;

#define POMODORO_TIME 25 * 60
#define SHORT_BREAK_TIME 5 * 60
#define LONG_BREAK_TIME 15 * 60
#define PROMODOROS_PER_LONG_BREAK 4

static const char *str_states[] = {"POMODORO", "BREAK"};
static const char *str_timer_states[] = {"stopped", "started"};

typedef struct {
  pomodorostatus state;
  kindof_timer pomodoro;
  u_int32_t timer;
  u_int32_t pomos;
  /* These are dirty buffers not to use but in gettimer and getstate funcs */
  char timer_buffer[6];
  char state_buffer[50];
} ktimer;

ktimer *new_ktimer();
void delete_ktimer(ktimer **p);
const char *getstate(ktimer *p);
void tick(ktimer *p);
void toggle(ktimer *p);
void stop(ktimer *p);
void start(ktimer *p);

ktimer *new_ktimer() {
  ktimer *p = (ktimer *)calloc(sizeof(ktimer), 1);
  p->pomodoro = POMODORO;
  p->state = STOPPED;
  p->timer = POMODORO_TIME;
  p->pomos = 0;
  return p;
}

void delete_ktimer(ktimer **p) {
  if (*p) {
    free(*p);
    *p = NULL;
  }
}

const char *gettimer(ktimer *p) {
  snprintf(p->timer_buffer, 6, "%02d:%02d", (int)p->timer / 60, p->timer % 60);
  return p->timer_buffer;
}

const char *getstate(ktimer *p) {
  const char *state =
      p->pomodoro == POMODORO ? str_states[POMODORO] : str_states[BREAK];
  const char *timer_state = str_timer_states[p->state];

  snprintf(p->state_buffer, 50, "%s %s %s", gettimer(p), state, timer_state);
  return p->state_buffer;
}

void toggle(ktimer *p) { p->state = p->state == STARTED ? STOPPED : STARTED; }
void stop(ktimer *p) { p->state = STOPPED; }
void start(ktimer *p) { p->state = STARTED; }

void tick(ktimer *p) {
  if (p->state == STOPPED)
    return;

  p->timer--;
  if (p->timer <= 0) {
    char buffer[2048];
    exec_command("paplay /home/jicg/Personal/KrtPomodoro/sounds/bell.ogg",
                 buffer);
    if (p->pomodoro == POMODORO) {
      p->pomos++;
      p->pomodoro = BREAK;
      p->timer = p->pomos % PROMODOROS_PER_LONG_BREAK == 0 ? LONG_BREAK_TIME
                                                           : SHORT_BREAK_TIME;
      exec_command("notify-send \"Pomodoro time is over. Take a rest\"",
                   buffer);
    } else { /* Break */
      exec_command(
          "notify-send -u critical \"Break time is over. Start working\"",
          buffer);
      p->timer = POMODORO_TIME;
      p->pomodoro = POMODORO;
      stop(p);
    }
  }
}

ktimer *local_pomodoro = NULL;

void get_builtin_pomodoro_status(char *buffer) {
  int max = 2048;
  if (!local_pomodoro)
    local_pomodoro = new_ktimer();
  tick(local_pomodoro);
  gettimer(local_pomodoro);
  char *color;
  if (local_pomodoro->state == STOPPED)
    color = pomodoro_paused;
  else if (local_pomodoro->pomodoro == POMODORO)
    color = pomodoro_color;
  else
    color = pomodoro_break;

  snprintf(buffer, max, "%s %s", color, local_pomodoro->timer_buffer);
}

void toggle_builtin_pomodoro() {
  if (local_pomodoro)
    toggle(local_pomodoro);
}
