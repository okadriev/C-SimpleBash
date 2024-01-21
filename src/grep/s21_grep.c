#include "s21_grep.h"

int main(int argc, char *argv[]) {
  int ERROR = 0;
  char *patterns = calloc(100, sizeof(char));

  Flags flag = get_flags(argc, argv, &ERROR, &patterns);

  if (!ERROR) {
    regex_t regex = precompiler(flag, patterns, &ERROR);

    while (optind < argc && !ERROR) read_file(argv[optind++], flag, regex);

    regfree(&regex);
  }

  free(patterns);
  return !ERROR;
}

Flags get_flags(int argc, char *argv[], int *ERROR, char **patterns) {
  int patterns_count = 0;
  Flags flag = {0};

  for (int i = 1; i < argc && !*ERROR; i++) {
    int opt;
    while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && !*ERROR) {
      switch (opt) {
        case 'e':
          add_pattern(optarg, patterns, &patterns_count);
          break;
        case 'i':
          flag.i = 1;
          break;
        case 'v':
          flag.v = 1;
          break;
        case 'c':
          flag.c = 1;
          break;
        case 'l':
          flag.l = 1;
          break;
        case 'n':
          flag.n = 1;
          break;
        case 'h':
          flag.h = 1;
          break;
        case 's':
          flag.s = 1;
          break;
        case 'f':
          pattern_file(optarg, patterns, &patterns_count);
          break;
        case 'o':
          flag.o = 1;
          break;
        default:
          fprintf(stderr, "grep: -%c: Unknown option\n", optopt);
          *ERROR = 1;
          break;
      }
    }
  }

  if (flag.l) flag.c = 0, flag.n = 0, flag.o = 0;

  if (flag.c) flag.n = 0, flag.o = 0;

  if (patterns_count == 0) {
    if (optind < argc) {
      add_pattern(argv[optind++], patterns, &patterns_count);
    } else
      printf("grep: pattern not found\n");
  }

  if (optind == argc) printf("grep: Add file\n");
  if (optind + 1 < argc) flag.multi_file = 1;

  return flag;
}

void pattern_file(char *optarg, char **patterns, int *patterns_count) {
  FILE *file = fopen(optarg, "r");

  if (file != NULL) {
    char *line = NULL;
    size_t len = 100;

    while (getline(&line, &len, file) != -1)
      add_pattern(line, patterns, patterns_count);

    free(line);
    fclose(file);
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);

    free(*patterns);
    exit(1);
  }
}

void add_pattern(char *line, char **patterns, int *patterns_count) {
  char *temp = strcpy(malloc(strlen(line) + 5), (line));

  if (temp[0] == '\0' || temp[0] == '\n') strcpy(temp, "$");
  if (temp[strlen(temp) - 1] == '\n') temp[strlen(temp) - 1] = '\0';

  pattern_line_realloc(patterns, temp);

  sprintf(*patterns + strlen(*patterns), "%s\\(%s\\)",
          *patterns_count ? "\\|" : "", temp);

  (*patterns_count)++;
  free(temp);
}

void pattern_line_realloc(char **patterns, char *optarg) {
  static size_t current_length = 100;

  while (strlen(*patterns) + strlen(optarg) + 10 > current_length) {
    current_length *= 2;
    *patterns = (char *)realloc(*patterns, current_length * sizeof(char *));
  }
}

regex_t precompiler(Flags flag, char *patterns, int *ERROR) {
  regex_t regex;
  // printf("%s\n(%ld)\n", patterns, strlen(patterns));  // help
  *ERROR = regcomp(&regex, patterns, (flag.i ? REG_ICASE : 0));
  if (*ERROR) fprintf(stderr, "regcomp error\n");

  return regex;
}

void read_file(char *filename, Flags flag, regex_t regex) {
  FILE *file = fopen(filename, "r");

  if (file != NULL) {
    if (!(flag.o && flag.v)) output(file, filename, flag, regex);

    fclose(file);
  } else if (!flag.s)
    fprintf(stderr, "grep: %s: No such file or directory\n", filename);
}

void output(FILE *file, char *filename, Flags flag, regex_t regex) {
  char *line = NULL;
  size_t len = 100;
  int correct_lines = 0;

  for (int line_count = 1; getline(&line, &len, file) != -1; line_count++) {
    int match = 0, len_line = strlen(line) - 1;

    if (line[len_line] == '\n') line[len_line] = '\0';

    if (flag.o)
      process_flag_o(line, line_count, filename, flag, regex);
    else
      match = !(regexec(&regex, line, 0, NULL, 0));

    if (match ^ flag.v) {
      correct_lines++;

      if (flag.l) {
        printf("%s\n", filename);
        break;

      } else if (!flag.c) {
        if (flag.multi_file && !flag.h) printf("%s:", filename);
        if (flag.n) printf("%d:", line_count);
        printf("%s\n", line);
      }
    }
  }

  if (flag.c) {
    if (flag.multi_file && !flag.h) printf("%s:", filename);
    printf("%d\n", correct_lines);
  }

  free(line);
  // line = NULL;
}

void process_flag_o(char *line, int line_count, char *filename, Flags flag,
                    regex_t regex) {
  regmatch_t match = {0};
  int curr_ch = 0;

  while (regexec(&regex, line + curr_ch, 1, &match, REG_NOTEOL) == 0) {
    if (flag.multi_file && !flag.h) printf("%s:", filename);

    if (flag.n) printf("%d:", line_count);

    char *temp = strdup(line + curr_ch + match.rm_so);
    temp[match.rm_eo - match.rm_so] = '\0';
    printf("%s\n", temp);

    free(temp);
    curr_ch += match.rm_eo;
  }
}