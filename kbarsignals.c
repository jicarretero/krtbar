#include "config.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

extern void toggle_pomodoro();

extern component_control *p_components;

// execvp(cmd, argv); //This will run "ls -la" as if it were a command
pid_t run(const char *cmd) {
  char *b = strndup(cmd, MAX_COMPONENT_BUFFER);
  char *tokens[1024] = {NULL};
  char *tok = strtok(b, " \t");
  int i = 0;
  tokens[i++] = tok;
  while ((tok = strtok(NULL, " \t"))) {
    tokens[i++] = tok;
  }
  pid_t p = fork();
  if (p == 0) {
    execvp(tokens[0], tokens);
  }
  return p;
}

void chldhandler(int sig) {
  pid_t pid = waitpid(-1, NULL, 0);
  size_t cl = sizeof(components) / sizeof(component);

  for (int i = 0; i < cl; i++) {
    if (pid == p_components[i].pid) {
      p_components[i].pid = 0;
      break;
    }
  }

  return;
}

void abrthandler(int sig) { abort(); }

void signal_handler(int signum, siginfo_t *info, void *context) {
  int blk = info->si_value.sival_int - 1;
  if (signum == SIGRTMIN) {
    component_control *c = &p_components[blk];
    if (c->pid == 0 && c->p_component->on_click)
      c->pid = run(c->p_component->on_click);
  }
}

void buttonhandler(int sig, siginfo_t *si, void *ucontext) {
  char button[2] = {'0' + si->si_value.sival_int & 0xff, '\0'};
  pid_t process_id = getpid();
  sig = si->si_value.sival_int >> 8;
  /* if (fork() == 0) {
    const Block *current;
    for (int i = 0; i < LENGTH(blocks); i++) {
      current = blocks + i;
      if (current->signal == sig)
        break;
    }
    char shcmd[1024];
    sprintf(shcmd, "%s && kill -%d %d", current->command, current->signal + 34,
            process_id);
    char *command[] = {"/bin/sh", "-c", shcmd, NULL};
    setenv("BLOCK_BUTTON", button, 1);
    setsid();
    execvp(command[0], command);
    exit(EXIT_SUCCESS);
  }*/
}

void setupsignals() {
  struct sigaction sa;
  printf("Setting up signals\n");

  for (int i = 0; i < 20; i++) {
    struct sigaction sa;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN + i, &sa, NULL);
    sigaddset(&sa.sa_mask, SIGRTMIN + i);
  }
}
