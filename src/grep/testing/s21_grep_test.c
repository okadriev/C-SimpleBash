#include "s21_grep_test.h"

int main(int argc, char* argv[]) {
  create_tests();

  if (argc > 1 && argv[1][0] >= '0' && argv[1][0] <= '5') {
    if (argc == 3 && (argv[2][0] == '0' || argv[2][0] == '1')) {
      start_testing(argv[1][0] - '0', argv[2][0] - '0');
    } else {
      start_testing(argv[1][0] - '0', 0);
    }
  } else if (argc == 2 && strcmp(argv[1], "girl") == 0) {
    paint_tyan();
  } else if (argc == 2 && strcmp(argv[1], "pika") == 0) {
    paint_pikachu(0);
  } else {
    int option = -1, valgrind = -1;

    printf("Add max num of flags to test (0-4)\n");
    while (option < 0 || option > 5) {
      option = getchar() - '0';
      if (option < 0 || option > 5) {
        printf("You're miss :( Try again, you can do it!\n");
        while (getchar() != '\n');
      }
    }

    printf("Also test with Valgrind? (0/1)\n");
    while (valgrind != 1 && valgrind != 0) {
      valgrind = getchar() - '0';
      if (option < 0 || option > 1) {
        printf("You're miss :( Try again, you can do it!\n");
        while (getchar() != '\n');
      }
    }

    start_testing(option, valgrind);
  }

  printf("\t\t\tCreated by huldades and lorindab\n");

  return 0;
}

void start_testing(int option, int valgrind) {
  unsigned seed = time(NULL);  // for "preset random" use fix number instead;
  srand(seed);

  for (int max_flags = 0; max_flags <= option; max_flags++) {
    int tests_count = 1;

    for (int i = 0; i < max_flags; i++) tests_count *= FLAGS_COUNT;

    for (int test_id = 0; test_id < tests_count; test_id++) {
      printf("\033[1;37mTest %3d / %3d: \033[0m", test_id + 1, tests_count);

      char line_flags[200] = {0};
      int copy_id = test_id, param = 1;
      char ch[2] = "x";

      for (int j = 0; j < max_flags; j++) {
        int flag_id = copy_id % FLAGS_COUNT;
        strcat(line_flags, flags[flag_id]);
        copy_id /= FLAGS_COUNT;

        if (flag_id == 8) {
          for (int i = rand() % 3 + 1; i > 0; i--) {
            do ch[0] = rand() % 128;
            while (!isalnum(ch[0]));

            strcat(line_flags, ch);
          }
          strcat(line_flags, " ");
          param = 0;

        } else if (flag_id == 9) {
          sprintf(line_flags + strlen(line_flags), PATH "test_%d.txt ",
                  rand() % 5 + 1);
          param = 0;
        }
      }

      for (int i = rand() % 3 + 1; param && i > 0; i--) {
        do ch[0] = rand() % 128;
        while (!isalnum(ch[0]));
        strcat(line_flags, ch);
      }

      strcat(line_flags, " ");
      system_call(line_flags, valgrind);

      int error = file_compare(valgrind);

      if (error) {
        paint_pikachu(1);

        if (max_flags > 0)
          printf("\033[1;37mWrong flag: %s\033[0m\n", line_flags);
        else
          printf("Wrong work without flags\n");

        printf("Current seed: %u\n", seed);
        printf("Output_compare: %s", error & 1 ? FAIL : SUCCESS);
        printf("Valgrind: %s", error & 2 ? FAIL : SUCCESS);

        exit(0);
      } else {
        printf(SUCCESS);
      }
    }
  }

  paint_tyan();
  system("rm -rf testing/test-samples/");
}

void system_call(char* line_flags, int valgrind) {
  char command[BUFSIZ] = {0};

  sprintf(command, "./s21_grep %s %s > %s", line_flags, TEST_FILES, S21_OUTPUT);
  printf("%s\n", command);
  system(command);

  sprintf(command, "grep %s %s > %s", line_flags, TEST_FILES, GREP_OUTPUT);
  printf("%s\n", command);
  system(command);

  if (valgrind) {
    sprintf(command, VALGRIND "./s21_grep %s %s > %s 2>&1", line_flags,
            TEST_FILES, VALGRIND_OUTPUT);
    printf("%s\n", command);
    system(command);
  }
}

int file_compare(int valgrind) {
  int error = 0;

  FILE* s21_file = fopen(S21_OUTPUT, "r");
  FILE* grep_file = fopen(GREP_OUTPUT, "r");

  while (1) {
    int ch1 = fgetc(s21_file);
    int ch2 = fgetc(grep_file);

    if (ch1 == EOF && ch2 == EOF) break;
    if (ch1 != ch2) {
      error = 1;
      break;
    }
  }

  fclose(s21_file);
  fclose(grep_file);

  if (valgrind) {
    FILE* valgrind_file = fopen(VALGRIND_OUTPUT, "r");

    const char noleaks[100] =
        "All heap blocks were freed -- no leaks are possible";
    char line[100] = {0};
    int no_leaks = 0;

    while (fgets(line, 100, valgrind_file) != NULL && !no_leaks)
      if (strstr(line, noleaks) != NULL) no_leaks = 1;

    if (!no_leaks) error += 2;

    fclose(valgrind_file);
  }

  return error;
}

void paint_pikachu(int param) {
  printf("\n");
  printf("\033[48;5;226m\033[1;30m⡏⠉⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⠀⠀⠀⠈⠛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠁⠀⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣧⡀⠀⠀⠀⠀⠙⠿⠿⠿⠻⠿⠿⠟⠿⠛⠉⠀⠀⠀⠀⠀⣸⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣷⣄⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣴⣿⣿⣿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣿⡟⠀⠀⢰⣹⡆⠀⠀⠀⠀⠀⠀⣭⣷⠀⠀⠀⠸⣿⣿⣿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣿⠃⠀⠀⠈⠉⠀⠀⠤⠄⠀⠀⠀⠉⠁⠀⠀⠀⠀⢿⣿⣿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣿⢾⣿⣷⠀⠀⠀⠀⡠⠤⢄⠀⠀⠀⠠⣿⣿⣷⠀⢸⣿⣿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣿⡀⠉⠀⠀⠀⠀⠀⢄⠀⢀⠀⠀⠀⠀⠉⠉⠁⠀⠀⣿⣿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿\033[0m\n");
  printf("\033[48;5;226m\033[1;30m⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿\033[0m\n");
  if (param) printf("\033[41m\033[1;37m        FAILED :(        \033[0m\n");
}

void paint_tyan() {
  printf("\033[48;5;93m\033[1;37m  ⠀⠀⠀⠀⠀⣀⡀⠀⠀⠀⠀⠀⡠⢴⣴⣾⣿⡿⠓⡠⠀⠀⠀⠀⠠⢄⠁⢀\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠀⠀⠀⠳⣽⡽⠀⠀⡠⢊⣴⣿⣿⣿⣡⠖⠁⣀⡤⢖⠟⠁⡠⠀⡙⢿⣷⣄\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠐⡀⠀⠀⠀⠀⢠⣾⣿⣿⢽⣿⣿⣿⣥⠖⣻⣯⡾⠃⠀⡔⡀⠀⣷⢸⢿⣿\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠀⢰⠀⠀⠀⢠⢟⣿⠃⢀⣾⣿⠟⠋⢀⡾⢋⣾⠃⣠⡾⢰⡇⡇⣿⣿⡞⣿\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⡤⣈⡀⠀⢀⠏⣼⣧⡴⣼⠟⠁⠀⠀⡾⠁⣾⡇⣰⢿⠃⢾⣿⣷⣿⣿⣇⢿\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠱⠼⠊⠀⠄⡜⣿⣿⡿⠃⠈⠁⠀⢸⠁⢠⡿⣰⢯⠃⠀⠘⣿⣿⣿⣿⣿⠸\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠀⠀⠀⠀⡘⡀⣸⣿⣱⡤⢴⣄⠀⠈⠀⠘⣷⠏⠌⠢⡀⠀⢿⣿⣿⣿⡟⡄\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠀⠀⢀⣌⠌⣴⣿⣿⠃⣴⣿⣟⡇⠀⠀⠀⠟⠀⠀⠀⠈⠢⢈⣿⡟⣿⡗⡇\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⢀⡴⡻⣡⣾⠟⢹⡇⠀⡇⢄⢿⠇⠀⠀⠀⠀⠀⠀⣽⣶⣄⡀⠘⢷⡹⣿⣿\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⣧⣾⡿⠋⠁⢀⡜⠙⡄⠓⠐⠁⠀⠀⠀⠀⠀⠀⡼⠛⠻⣟⠛⣆⠈⢷⣿⣿\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⣴⣾⣟⣵⣿⣿⣿⣁⢇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡧⠠⠔⡹⠀⢸⠀⣼⣿⣿\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠿⡽⢫⡉⠀⣠⠔⠁⡀⠕⠠⡀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠖⠊⠀⠀⢊⣾⢿⡿⠉\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠁⠀⡹⢨⠁⠐⠈⢀⡠⠐⠁⠄⠡⡀⡀⠀⠀⠀⠀⠀⠀⠀⠠⠶⢛⡨⠊⠀⠀\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⡜⠀⠈⣂⠀⠀⠀⠀⡠⠐⠉⡆⠀⣀⢀⣀⣀⣀⡀⠀⠀⣀⠴⣁⡀⠤⠀⠀\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠈⠀⠀⠀⡇⠑⢄⠀⠀⠀⠀⣲⢥⡎⠀⢰⠀⢸⠀⢀⠉⠙⣿⣧⣀⣀⣂⣤⣼\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠀⠆⠁⠃⠀⠀⠈⠒⠒⠊⣸⠚⠁⠀⠀⠀⠀⠀⠀⠀⡜⠁⠀⠀⠀⠀⠈⠚\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠀⠀⠀⠂⠀⠀⠀⠀⠀⢀⠋⢆⠀⠀⠀⠀⠀⠀⠀⡘⠀⠀⠀⠀⠀⠀⠀⠀\033[0m\n");
  printf("\033[48;5;93m\033[1;37m⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠒⠂⠀⠀⠐⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀\033[0m\n");
  printf("\033[42m\033[1;37m   YOU'RE AWESOME!  \\(^.^)/   \033[0m\n");
}

void create_tests() {
  create_dir();

  create_test_1();
  create_test_2();
  create_test_3();
  create_test_4();
  create_test_5();
}

void create_dir() {
  mkdir("testing", 0777);
  if (mkdir("testing/test-samples", 0777) == 0) {
    printf("Folder created\n");
  }
}

void create_test_1() {
  FILE* f = fopen(PATH "test_1.txt", "w");
  fprintf(f, "\nabc end\n\n\nabcabcabc end\n\n\n\nabcabcabcabc end\n\n");
  fclose(f);
}

void create_test_2() {
  FILE* f = fopen(PATH "test_2.txt", "w");
  fprintf(f, "tab	tab	end\n\nabc tab	1abc end\nabc end\n\n");
  fclose(f);
}

void create_test_3() {
  FILE* f = fopen(PATH "test_3.txt", "w");

  /* for (int ch = 0; ch < 128; ch++) {  // ascii easy mode
    fprintf(f, "%c", ch);
  } */
  for (int i = 0; i < 2000; i++) {
    int ch = rand() % 128;
    while (!isalnum(ch)) ch = rand() % 128;  // HARD MODE
    fprintf(f, "%c", ch);
    if (i % 10 == 9) fprintf(f, "\n");
  }

  fclose(f);
}

void create_test_4() {
  FILE* f = fopen(PATH "test_4.txt", "w");
  fprintf(f,
          "	\n    		\n            			\n             "
          "           				\n                             "
          "           					");
  fclose(f);
}

void create_test_5() {
  FILE* f = fopen(PATH "test_5.txt", "w");
  fprintf(f, " ");  // easy mode
  // fprintf(f, ""); // HARD MODE
  fclose(f);
}