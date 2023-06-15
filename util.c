#include "util.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

int pscanf(const char *path, const char *fmt, ...) {
  FILE *fp = NULL;
  va_list ap;
  int n;

  fp = fopen(path, "r");

  if (fp == NULL) {
    fprintf(stderr, "ERROR opening file %s - error=%d\n", path, errno);
    return -1;
  }
  va_start(ap, fmt);
  n = vfscanf(fp, fmt, ap);
  va_end(ap);
  fclose(fp);

  return (n == EOF) ? -1 : 0;
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

  /* Remove \n as the last character after running the script ...*/
  int l = strlen(buffer) - 1;
  buffer[l] = buffer[l] == '\n' ? 0 : buffer[l];

  /* close */
  pclose(fp);
}
