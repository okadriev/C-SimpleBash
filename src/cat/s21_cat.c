#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Flags flag = {0};
  int Error = 0;

  flag = get_flag(argc, argv, flag, &Error);

  for (int i = optind; i < argc && !Error; i++) print_file(argv[i], flag);

  return Error;
}

Flags get_flag(int argc, char *argv[], Flags flag, int *Error) {
  int opt;
  while ((opt = getopt_long(argc, argv, "bnseEtTv", long_opt, NULL)) != -1 &&
         !*Error)
    switch (opt) {
      case 'b':
        flag.b = 1;
        break;
      case 'n':
        flag.n = 1;
        break;
      case 's':
        flag.s = 1;
        break;
      case 'e':
        flag.v = 1;
        flag.e = 1;
        break;
      case 'E':
        flag.e = 1;
        break;
      case 't':
        flag.v = 1;
        flag.t = 1;
        break;
      case 'T':
        flag.t = 1;
        break;
      case 'v':
        flag.v = 1;
        break;
      default:
        *Error = 1;
    }
  if (flag.b) flag.n = 0;

  return flag;
}

void print_file(char *filename, Flags flag) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", filename);
  } else
    print_ch(file, flag);

  fclose(file);
}

void print_ch(FILE *file, Flags flag) {
  static int ch_prev_2 = ' ', ch_prev = '\n', ch = 0, str_count = 0;

  while ((ch = fgetc(file)) != EOF) {
    if (flag.s && ch_prev_2 == '\n' && ch_prev == '\n' && ch == '\n') continue;

    if (flag.b && ch != '\n' && ch_prev == '\n') printf("%6d\t", ++str_count);

    if (flag.n && ch_prev == '\n') printf("%6d\t", ++str_count);

    if (flag.e && ch == '\n') printf("$");

    if (flag.t && ch == '\t') {
      printf("^");
      ch += 64;
    }

    if (flag.v && ((ch < 32 && ch != '\n' && ch != '\t') || ch >= 127)) {
      if (ch > 127) {  // METACHAR
        printf("M-");
        ch -= 128;
      }

      if (ch < 32 || ch == 127) {
        printf("^");
        ch = ((ch == 127) ? ch - 128 + 64 : ch + 64);
      }
    }

    printf("%c", ch);
    ch_prev_2 = ch_prev;
    ch_prev = ch;
  }
}