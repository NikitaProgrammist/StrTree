#include <stdio.h>
#include <stdarg.h>

#include "my_printf.h"

void colorPrintf(Colors color, Fonts font, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf("\033[%d;%dm", color, font);
  vprintf(fmt, args);
  printf("\033[0m\n");
  va_end(args);
}
