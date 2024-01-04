// TODO: парсинг параметров после файла, makefile путь exe в билд и убрать ехе
#include "s21_cat.h"

int main(int arg_count, char *argument[]) {
  flags flag = {0};
  int wrong_flag = 0;

  for (int i = 1; i < arg_count && !wrong_flag; i++) {
    if (argument[i][0] == '-') flag = get_flag(argument[i], flag, &wrong_flag);
  }

  for (int i = 1; i < arg_count && !wrong_flag; i++) {
    if (argument[i][0] != '-') print_f(argument[i], flag);
  }

  return 0;
}

flags get_flag(char arg[], flags flag, int *wrong_flag) {
  int i = 0;
  if (arg[1] == '-') {
    if (strcmp(arg, "--number-nonblank") == 0)
      flag.b = 1;
    else if (strcmp(arg, "--number") == 0)
      flag.n = 1;
    else if (strcmp(arg, "--squeeze-blank") == 0)
      flag.s = 1;
    else {
      fprintf(stderr, "cat: %s: Unknown option\n", arg);
      *wrong_flag = 1;
    }

  } else
    while (arg[++i] != '\0' && !*wrong_flag) {
      switch (arg[i]) {
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
          fprintf(stderr, "cat: -%c: Unknown option\n",
                  arg[i]);  // потестить эту ошибку с большим колвом аргументов
          *wrong_flag = 1;
          break;
      }
    }
  if (flag.b) flag.n = 0;

  return flag;
}

void print_f(char *fil, flags flag) {
  FILE *f = fopen(fil, "r");
  if (f == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", fil);
  }

  static int ch0 = ' ', ch1 = '\n', ch2;

  while ((ch2 = fgetc(f)) != EOF) {
    print_ch(ch0, ch1, ch2, flag);
    ch0 = ch1;
    ch1 = ch2;
  }

  fclose(f);
}

void print_ch(int ch0, int ch1, int ch2, flags flag) {
  if (flag.s == 0 || ch0 != '\n' || ch1 != '\n' || ch2 != '\n') {
    static int str_count = 0;

    if (flag.b && ch2 != '\n' && ch1 == '\n') printf("%6d\t", ++str_count);

    if (flag.n && ch1 == '\n') printf("%6d\t", ++str_count);

    if (flag.e && ch2 == '\n') printf("$");

    if (flag.t && ch2 == '\t') {
      printf("^");
      ch2 += 64;
    }

    if (flag.v && ((ch2 < 32 && ch2 != '\n' && ch2 != '\t') || ch2 >= 127)) {
      if (ch2 > 127) {  // METACHAR
        printf("M-");
        ch2 -= 128;
      }

      if (ch2 < 32 || ch2 == 127) {
        printf("^");
        ch2 = ((ch2 == 127) ? ch2 - 128 + 64 : ch2 + 64);
      }
    }

    printf("%c", ch2);
  }
}