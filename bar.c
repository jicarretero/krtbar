#include "colors.h"
#include "config.h"
#include "util.h"
#include <X11/Xlib.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef __uintmax_t uintmax_t;

void gw(char *buffer) {}
extern void get_pomodoro_status(char *buffer);
extern void setupsignals();
extern void abrthandler();
extern void chldhandler();

component_control *p_components;

void get_date_time(char *buffer) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);

  size_t ret =
      strftime(buffer, MAX_COMPONENT_BUFFER, " %d-%m-%Y %H:%M:%S", tm);
  /* strftime(buffer, MAX_COMPONENT_BUFFER, " %a %d-%m-%Y %H:%M:%S", tm);*/
}

void exec_command(const char *command, char *buffer) {
  FILE *fp;

  /* Open the command for reading. */
  fp = popen(command, "r");
  if (fp == NULL) {
    printf("Failed to run command\n");
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(buffer, MAX_COMPONENT_BUFFER, fp) != NULL)
    ;

  /* close */
  pclose(fp);
}

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
  /* memtotal = memtotal / 1024;
  printf("%d %ju %ju %ju %ju\n", n, memtotal, memfree, buffers, cached);
  printf("USED: %ju\n", used); */

  return used;
}

void get_used_mem(char *buffer) {
  snprintf(buffer, MAX_COMPONENT_BUFFER, " %lu Mb", read_memory());
}

void get_battery_state(char *buffer) {
  char *bs[] = {"", "", "", "", ""};
  int mx[] = {90, 60, 40, 20, 10};
  int capacity;
  char status[12];
  pscanf("/sys/class/power_supply/BAT0/capacity", "%d", &capacity);
  pscanf("/sys/class/power_supply/BAT0/status", "%12[a-zA-Z ]", &status);

  char *r;
  char *s;

  for (int j = 0; j < 5; j++) {
    if (capacity > mx[j]) {
      r = bs[j];
      break;
    }
  }

  if (strncmp(status, "Discharging", 12) == 0)
    s = ""; /* "🔋"; */
  else
    s = "";

  snprintf(buffer, MAX_COMPONENT_BUFFER, "%s %s %d%%", s, r, capacity);
}

Display *dpy = NULL;
Window root;

void write_status(const char *buff) {
  XStoreName(dpy, root, buff);
  XFlush(dpy);
}

void openx() {
  dpy = XOpenDisplay(NULL);
  int screen = DefaultScreen(dpy);
  root = RootWindow(dpy, screen);

  if (dpy == NULL) {
    fprintf(stderr, "ERROR: Can't open Display\n");
    return;
  }
}

void closex() { XCloseDisplay(dpy); }

int main_loop() {
  sleep(2);
  setlocale(LC_CTYPE, "");
  signal(SIGSEGV, abrthandler);

  char full_buffer[2048];

  openx();

  size_t cl = sizeof(components) / sizeof(component);

  /* const char *buffer_weather = components[0].buffer;
  const char *buffer_pomodoro = components[1].buffer;
  const char *battery_buffer = components[2].buffer;
  const char *mem_buffer = components[3].buffer;
  const char *tm_buffer = components[4].buffer; */

  do {
    char cr[] = {(char)31, '\0'};
    char output[8192] = {0};
    component_control *cc = (component_control *)p_components;
    char fgfoocolor[20] = {};
    for (int i = 0; i < cl; i++, cc++) {
      const component *c = cc->p_component;
      char bg[10];
      strncpy(bg, c->bg_color, 10);
      if (c->fn)
        c->fn(cc->buffer);
      else if (c->s)
        exec_command(c->s, cc->buffer);

      strncat(output, bg, 8000);
      bg[1] = 'c';
      strncat(output, bg, 8000);
      strncat(output, cr, 8000);
      strncat(output, " ", 8000);
      strncat(output, c->fg_color, 8000);
      strncat(output, cc->buffer, 8000);
      strncat(output, " ", 8000);
    }

    write_status(output);
    sleep(1);

    /* char *s = "🔌 🔋   JIKONIK Hola mundo mundial!"; */
  } while (1);
  return 0;
}

int main() {
  int pid;

  setupsignals();
  signal(SIGCHLD, chldhandler);

  size_t cl = sizeof(components) / sizeof(component);

  p_components = calloc(cl, sizeof(component_control));

  for (int i = 0; i < cl; i++) {
    p_components[i].p_component = &components[i];
  }

  main_loop();
}
