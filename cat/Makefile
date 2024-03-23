GCC=gcc -Wall -Wextra -Werror -std=c11

all: s21_cat

clean:
	rm -rf s21_cat

rebuild: clean clang s21_cat

s21_cat: s21_cat.c s21_cat.h
	$(GCC) -o s21_cat s21_cat.c

clang:
	cp ../../materials/linters/.clang-format .clang-format
	clang-format -i s21_cat.c