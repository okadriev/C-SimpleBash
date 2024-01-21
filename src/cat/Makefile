SHELL = /bin/sh

all: clean s21_cat test_compile
s21_cat:
	gcc -Wall -Wextra -Werror -std=c11 s21_cat.c -o s21_cat

test_compile: 
	gcc -Wall -Wextra -Werror -std=c11 testing/s21_cat_test.c -o cat_test

test:
	./cat_test 1

re: clean all
clean:
	rm -rf s21_cat cat_test