#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

extern int errno;

#define pomodoro_color "^c#30f0ff^"
#define pomodoro_paused "^c#ff0000^"
#define pomodoro_break "^c#ffff00^"

int get_connection() {
  struct sockaddr_in server, cli;
  int s;
  int port = 4884;
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    exit(3);
  }
  // int flags = fcntl(s, F_GETFL, 0);
  //  fcntl(s, F_SETFL, flags | O_NONBLOCK);

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(4884);

  if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
    exit(4);
  }
  return s;
}

void close_connection(int s) { close(s); }

void get_pomodoro_status(char *buffer) {
  int max = 2048;
  char *buf = "GET /pomodoro\r\n\n";
  char recbuf[2048] = {0};
  int s = get_connection();
  write(s, buf, strlen(buf));
  int b = read(s, recbuf, 2048);

  if (b == -1)
    snprintf(buffer, max, " NO POMODORO %d", errno);
  else {
    char *pm_t = strtok(recbuf, " "); /* time */
    char *pm_s = strtok(NULL, " ");   /* stopped or started */
    char *pm_w = strtok(NULL, " ");   /* POMODORO or stopped */
    char *color;
    if (strncmp(pm_s, "stopped", 8) == 0)
      color = pomodoro_paused;
    else if (strncmp(pm_w, "POMODORO", 9) == 0)
      color = pomodoro_color;
    else
      color = pomodoro_break;

    snprintf(buffer, max, "%s %s", color, pm_t);
  }
  close_connection(s);
}

void toggle_pomodoro() {
  char *buf = "POST /pomodoro/toggle HTTP/1.1\r\n\n";
  char recbuf[2048];
  int s = get_connection();
  write(s, buf, strlen(buf));
  int b = read(s, recbuf, 2048);
  // printf("B:%s\n", recbuf);
  close_connection(s);
}
