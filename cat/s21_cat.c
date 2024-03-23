#include "s21_cat.h"

int main(int argc, char *argv[]) {
  processing(argc, argv);
  return 0;
}

int shortOptions(char *argv[], int count, opt *options) {
  for (int i = 1; i < (int)strlen(argv[count]); i++) {
    switch (argv[count][i]) {
      case 'b':
        options->b_numForNotEmpty = 1;
        options->n_numForAll = 0;
        break;
      case 'e':
        options->e_endDollar = 1;
        options->v_showHiddens = 1;
        break;
      case 'E':
        options->e_endDollar = 1;
        break;
      case 'n':
        options->n_numForAll = options->b_numForNotEmpty == 1 ? 0 : 1;
        break;
      case 's':
        options->s_squeeze = 1;
        break;
      case 't':
        options->t_showTabs = 1;
        options->v_showHiddens = 1;
        break;
      case 'T':
        options->t_showTabs = 1;
        break;
      case 'v':
        options->v_showHiddens = 1;
        break;
      default:
        perror(*argv);
        return 0;
    }
  }
  return 1;
}

int longOptions(char *argv[], int count, opt *options) {
  if (strcmp(argv[count], "--number-nonblank") == 0) {
    options->b_numForNotEmpty = 1;
    options->n_numForAll = 0;
  } else if (strcmp(argv[count], "--number") == 0) {
    options->n_numForAll = 1;
  } else if (strcmp(argv[count], "--squeeze-blank") == 0) {
    options->s_squeeze = 1;
  } else {
    perror(*argv);
    return 0;
  }
  return 1;
}

int parseArgs(opt *options, char *argv[], int argc, int *index) {
  int count = 1;
  while (count != argc) {
    if (argv[count][0] == '-' && argv[count][1] != '-') {
      if (!shortOptions(argv, count, options)) return 0;
    } else if (argv[count][1] == '-') {
      if (!longOptions(argv, count, options)) return 0;
    } else {
      *index = count;
      return 1;
    }
    count++;
  }
  return 1;
}

void getsymb(int c, int *prev, opt options, int *index, int *eline_printed) {
  if (!(options.s_squeeze != 0 && *prev == '\n' && c == '\n' &&
        *eline_printed)) {
    if (*prev == '\n' && c == '\n')
      *eline_printed = 1;
    else
      *eline_printed = 0;
    if (((options.n_numForAll != 0 && options.b_numForNotEmpty == 0) ||
         (options.b_numForNotEmpty != 0 && c != '\n')) &&
        *prev == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }
    if (options.e_endDollar != 0 && c == '\n') printf("$");
    if (options.t_showTabs != 0 && c == '\t') {
      printf("^");
      c = '\t' + 64;
    }
    if (options.v_showHiddens != 0 && c >= 0 && c <= 31 && c != '\n' &&
        c != '\t') {
      printf("^");
      c = c + 64;
    }
    if (options.v_showHiddens != 0) {
      if (c > 127 && c < 160) printf("M-^");
      if ((c < 32 && c != '\n' && c != '\t') || c == 127) printf("^");
      if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t')
        c = c > 126 ? c - 128 + 64 : c + 64;
    }
    fputc(c, stdout);
  }
  *prev = c;
}

int printFile(char *name, opt options) {
  int errCode = 1;
  FILE *file = fopen(name, "r");
  if (file != NULL) {
    int index = 0;
    int elinePrinted = 0;
    int c = fgetc(file), prev = '\n';
    while (c != EOF) {
      getsymb(c, &prev, options, &index, &elinePrinted);
      c = fgetc(file);
    }
    fclose(file);
  } else {
    errCode = 0;
  }
  return errCode;
}

void processing(int argc, char *argv[]) {
  int index = 0;
  opt options = {0};
  int check = parseArgs(&options, argv, argc, &index);
  while (index < argc && check) {
    int fileError = printFile(argv[index], options);
    if (!fileError) {
      perror(argv[index]);
    }
    index++;
  }
}
