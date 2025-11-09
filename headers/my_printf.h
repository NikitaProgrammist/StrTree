#ifndef MY_PRINTF_H
#define MY_PRINTF_H

enum Colors {
  BLACK = 30,
  RED,
  GREEN,
  YELLOW,
  CYAN,
  PURPLE,
  BLUE,
  WHITE,
};

enum Fonts {
  BOLD = 1,
  FAINT,
  ITALIC,
  UNDERLINE,
  CROSSED = 9,
  PRIMARY
};

void colorPrintf(Colors color, Fonts font, const char *fmt, ...);

#endif // MY_PRINTF_H
