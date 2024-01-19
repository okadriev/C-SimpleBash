#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Flags {
  int i, v, c, l, n, h, s, o, multi_file, empty_file;
} Flags;

Flags get_flags(int argc, char *argv[], int *ERROR, char **patterns);
void pattern_file(char *optarg, char **patterns, int *patterns_count);
void add_pattern(char *optarg, char **patterns, int *patterns_count);
void pattern_line_realloc(char **patterns, char *optarg);
regex_t precompiler(Flags flag, char *patterns, int *ERROR);
void read_file(char *filename, Flags flag, regex_t regex);
void output(FILE *file, char *filename, Flags flag, regex_t regex);
void process_flag_o(char *line, int line_count, char *filename, Flags flag,
                    regex_t regex);