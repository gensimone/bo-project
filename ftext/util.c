#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  It prints a formatted error message (optionally including errno)
  to stderr and then terminates the program.
  Stolen from util.c at https://git.suckless.org/dwm
*/
void die(const char* fmt, ...)
{
  va_list ap;
  int saved_errno;
  saved_errno = errno;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  if (fmt[0] && fmt[strlen(fmt) - 1] == ':')
    fprintf(stderr, " %s", strerror(saved_errno));
  fputc('\n', stderr);
  exit(EXIT_FAILURE);
}
