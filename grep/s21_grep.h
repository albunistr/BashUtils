#ifndef S21_GREP_H
#define S21_GREP_H
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct options {
  int e_pattern;
  int i_ignorRegistr;
  int v_withoutPattern;
  int c_countPattens;
  int l_namesOfFiles;
  int n_numOfLine;
  char* patterns[100];
  int count_of_args;
} opt;

void parser(int argc, char* argv[], opt* options);
void c_with_l(opt options, int counterMatches);
void output(char* argv[], opt options, int mof);
void processing(int argc, char* argv[]);

#endif