GCC=gcc -Wall -Wextra -Werror -std=c11

all: s21_grep

clean:
	rm -rf s21_grep

rebuild: clean clang s21_grep

s21_grep: s21_grep.c s21_grep.h
	$(GCC) -o s21_grep s21_grep.c

clang:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -i s21_grep.c