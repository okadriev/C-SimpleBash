#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flags {
  int b, e, n, s, t, v;
} flags;

flags get_flag(char c[], flags flag);
void print_f(char *fil, flags flag);