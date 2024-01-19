#include "s21_grep_test.h"

int main(int argc, char* option[]) {
  int param = -1;
  create_tests();
  if (argc == 2 && option[1][0] >= '0' && option[1][0] <= '5') {
    start_testing(option[1][0]);
  } else {
    while (param < 0 || param > 5) {
      printf("Add num of flags 0-4)\n");
      scanf("%d", &param);
    }
    start_testing(param);
  }

  return 0;
}

void start_testing(int option) {
  for (int max_flags = 0; max_flags <= option - '0'; max_flags++) {
    int tests_count = 1;

    for (int i = 0; i < max_flags; i++) tests_count *= FLAGS_COUNT;

    for (int test_id = 0; test_id < tests_count; test_id++) {
      printf("\033[1;37mTest %3d / %3d: \033[0m", test_id + 1, tests_count);
      srand(time(NULL));

      char line_flags[200] = {0};
      int copy_id = test_id, param = 1;
      char ch[2] = {0, '\0'};  // grep
      for (int j = 0; j < max_flags; j++) {
        int flag_id = copy_id % FLAGS_COUNT;
        strcat(line_flags, flags[flag_id]);

        copy_id /= FLAGS_COUNT;

        if (flag_id == 8) {                                              // grep
          for (int i = rand() % 3 + 1; i > 0; i--) {                     // grep
            do ch[0] = rand() % 128;                                     // grep
            while (!isalnum(ch[0]));                                     // grep
            strcat(line_flags, ch);                                      // grep
          }                                                              // grep
          strcat(line_flags, " ");                                       // grep
          param = 0;                                                     // grep
        }                                                                // grep
        if (flag_id == 9) {                                              // grep
          sprintf(line_flags + strlen(line_flags), PATH "test_%d.txt ",  // grep
                  rand() % 5 + 1);                                       // grep
          param = 0;                                                     // grep
        }                                                                // grep
      }
      if (param)                                    // grep
        for (int i = rand() % 3 + 1; i > 0; i--) {  // grep
          do ch[0] = rand() % 128;                  // grep
          while (!isalnum(ch[0]));                  // grep
          strcat(line_flags, ch);                   // grep
        }                                           // grep

      strcat(line_flags, " ");
      system_call(line_flags);

      int error = file_compare();

      if (error) {
        paint_pikachu();

        // if (max_flags > 0)  // grep
        printf("\033[1;37mWrong flag: %s\033[0m\n", line_flags);
        // else                                     // grep
        //   printf("Wrong work without flags\n");  // grep
        printf("Output_compare: %s\n", error & 1 ? FAIL : SUCCESS);
        printf("Valgrind: %s\n", error & 2 ? FAIL : SUCCESS);

        exit(1);
      }

      if (!error) printf(SUCCESS);
    }
  }

  paint_tyan();
  system("rm -rf testing/test-samples/");
}

void system_call(char* line_flags) {
  char command[BUFSIZ] = {0};

  sprintf(command, "./s21_grep %s %s > %s", line_flags, TEST_FILES, S21_OUTPUT);
  printf("%s\n", command);
  system(command);

  sprintf(command, "grep %s %s > %s", line_flags, TEST_FILES, GREP_OUTPUT);
  printf("%s\n", command);
  system(command);

  sprintf(command, VALGRIND "./s21_grep %s %s > %s 2>&1", line_flags,
          TEST_FILES, VALGRIND_OUTPUT);
  printf("%s\n", command);
  system(command);
}

int file_compare() {
  int error = 0;

  FILE* s21_file = fopen(S21_OUTPUT, "r");
  FILE* grep_file = fopen(GREP_OUTPUT, "r");
  FILE* valgrind_file = fopen(VALGRIND_OUTPUT, "r");

  while (1) {
    int ch1 = fgetc(s21_file);
    int ch2 = fgetc(grep_file);

    if (ch1 == EOF && ch2 == EOF) break;
    if (ch1 != ch2) {
      error = 1;
      break;
    }
  }

  char str_find[100] = "All heap blocks were freed -- no leaks are possible";
  char str_read[100] = {0};
  while (fgets(str_read, 100, valgrind_file) != NULL) {
    if (strstr(str_find, str_read) != 0) {
      error += 2;
      break;
    }
  }

  fclose(s21_file);
  fclose(grep_file);
  fclose(valgrind_file);

  return error;
}

void paint_pikachu() {
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
  printf("\033[41m\033[1;37m        FAILED :(        \033[0m\n");
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

  /* for (int ch = 0; ch < 128; ch++) {  // ascii
    fprintf(f, "%c", ch);
  } */
  for (int i = 0; i < 1000; i++) {
    int ch = rand() % 128;                   // grep
    while (!isalnum(ch)) ch = rand() % 128;  // grep
    fprintf(f, "%c", ch);                    // grep
    if (i % 10 == 9) fprintf(f, "\n");       // grep
  }                                          // grep

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
  fprintf(f, " ");
  fclose(f);
}