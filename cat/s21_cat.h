#ifndef S21_CAT_H
#define S21_CAT_H
#include <stdio.h>
#include <string.h>

typedef struct options {
  int b_numForNotEmpty;
  int e_endDollar;
  int n_numForAll;
  int s_squeeze;
  int t_showTabs;
  int v_showHiddens;
} opt;

int parseArgs(opt *options, char *argv[], int argc, int *index);
void getsymb(int c, int *prev, opt options, int *index, int *eline_printed);
int printFile(char *name, opt options);
void processing(int argc, char *argv[]);
int longOptions(char *argv[], int count, opt *options);
int shortOptions(char *argv[], int count, opt *options);

#endif