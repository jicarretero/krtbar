#include <signal.h>
#include <stdio.h>
#include <unistd.h>

extern void toggle_pomodoro();

// execvp(cmd, argv); //This will run "ls -la" as if it were a command

void run(char *cmd) {
  char *a[2];
  a[0] = cmd;
  a[1] = NULL;
  pid_t p = fork();
  if (p == 0) {
    execvp(cmd, a);
  }
}

void signal_handler(int signum, siginfo_t *info, void *context) {
  /* printf("Received signal: %d and info %d [%d]\n", signum,
         info->si_value.sival_int, SIGRTMIN); */
  int blk = info->si_value.sival_int;
  if (signum == SIGRTMIN)
    if (blk == 2)
      toggle_pomodoro();
  if (blk == 5)
    run("/usr/bin/pavucontrol");
  if (blk == 6)
    run("/usr/bin/gsimplecal");
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

  for (int i = 0; i < 20; i++) {
    struct sigaction sa;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN + i, &sa, NULL);
    sigaddset(&sa.sa_mask, SIGRTMIN + i);
  }
}
