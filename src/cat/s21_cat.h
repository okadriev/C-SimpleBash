#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Flags {
  int b, e, n, s, t, v;
} Flags;

static struct option long_opt[] = {{"number-nonblank", 0, NULL, 'b'},
                                   {"number", 0, NULL, 'n'},
                                   {"squeeze-blank", 0, NULL, 's'},
                                   {0, 0, 0, 0}};

Flags get_flag(int argc, char *argv[], Flags flag, int *Error);
void print_file(char *filename, Flags flag);
void print_ch(FILE *file, Flags flag);