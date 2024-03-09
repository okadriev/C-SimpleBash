SHELL = /bin/sh

all: clean test

s21_grep:
	gcc -Wall -Wextra -Werror -std=c11 s21_grep.c -o s21_grep

test:
	gcc -Wall -Wextra -Werror -std=c11 s21_grep.c -o s21_grep
	gcc -Wall -Wextra -Werror -std=c11 testing/s21_grep_test.c -o grep_test
	./grep_test 2

re: clean all
clean:
	rm -rf s21_grep grep_test