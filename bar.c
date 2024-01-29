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
#include <unistd.h>

void gw(char *buffer) {}
extern void setupsignals();
extern void abrthandler();
extern void chldhandler();

component_control *p_components;

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

  u_int64_t tick = 0;

  do {
    char cr[] = {(char)31, '\0'};
    char output[8192] = {0};
    component_control *cc = (component_control *)p_components;
    char fgfoocolor[20] = {};
    for (int i = 0; i < cl; i++, cc++) {
      const component *c = cc->p_component;
      char bg[10];
      strncpy(bg, c->bg_color, 10);
      if (tick % c->upd_time == 0) {
        if (c->fn)
          c->fn(cc->buffer);
        else if (c->s)
          exec_command(c->s, cc->buffer);
      }

      strncat(output, bg, 8000);
      bg[1] = 'c';
      strncat(output, bg, 8000);
      strncat(output, cr, 8000);
      strncat(output, c->fg_color, 8000);
      strncat(output, separator, 8000);
      strncat(output, cc->buffer, 8000);
      strncat(output, c->fg_color, 8000);
      strncat(output, " ", 8000);
    }

    write_status(output);
    sleep(1);
    tick++;

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
