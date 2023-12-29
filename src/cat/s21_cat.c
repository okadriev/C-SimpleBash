#include "s21_cat.h"

int main(int argcount, char *argument[]) {
  flags flag = {0};

  for (int i = 1; i < argcount; i++) {
    if (argument[i][0] == '-') {
      flag = get_flag(argument[i], flag);
    } else {
      print_f(argument[i], flag);
    }
  }

  return 0;
}

flags get_flag(char c[], flags flag) {
  int i = 0;
  if (c[1] == '-') {
    if (strcmp(c, "--number-nonblank") == 0) flag.b = 1;
    if (strcmp(c, "--number") == 0) flag.n = 1;
    if (strcmp(c, "--squeeze-blank") == 0) flag.s = 1;
  } else
    while (c[++i] != '\0') {
      switch (c[i]) {
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
        default:
          fprintf(stderr, "cat: -%c: Unknown option\n", c[i]);
          break;
      }
    }
  return flag;
}

void print_f(char *fil, flags flag) {
  // printf("b = %d, e = %d, n = %d, s = %d, t = %d, v = %d\n", flag.b, flag.e,
  // flag.n, flag.s, flag.t,
  //        flag.v);
  FILE *f = fopen(fil, "r");
  if (f == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", fil);
    // exit(1);
  }

  int ch0 = ' ', ch1 = '\n', ch2, str_count = 0;
  while ((ch2 = fgetc(f)) != EOF) {
    if (flag.b && ch2 != '\n' && ch1 == '\n')
      printf("%d ", ++str_count);
    else if (flag.n && ch1 == '\n')
      printf("%d ", ++str_count);

    if (flag.s == 0 || ch0 != '\n' || ch1 != '\n' || ch2 != '\n') {
      if (flag.e && ch2 == '\n') printf("$");
      if (flag.t && ch2 == '\t')
        printf("^I");
      else if (flag.v &&
               ((ch2 < 32 && ch2 != '\n' && ch2 != '\t') || ch2 == 127))
        // if (ch2 > 127 && ch2 < 160) printf("M-^");
        printf("^%c", ch2 == 127 ? ch2 - 128 + 64 : ch2 + 64);
      // if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t') c = c >
      // 126 ? c - 128 + 64 : c + 64;
      else
        printf("%c", ch2);
    }

    ch0 = ch1;
    ch1 = ch2;
  }

  fclose(f);
}