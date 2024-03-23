#include "s21_grep.h"

int main(int argc, char* argv[]) {
  processing(argc, argv);
  return 0;
}

void parser(int argc, char* argv[], opt* options) {
  int current_char = 0;
  while ((current_char = getopt_long(argc, argv, "e:ivcln", NULL, 0)) != -1) {
    switch (current_char) {
      case 'e':
        options->e_pattern = 1;
        options->patterns[options->count_of_args++] = optarg;
        break;
      case 'i':
        options->i_ignorRegistr = 1;
        break;
      case 'v':
        options->v_withoutPattern = 1;
        break;
      case 'c':
        options->c_countPattens = 1;
        break;
      case 'l':
        options->l_namesOfFiles = 1;
        break;
      case 'n':
        options->n_numOfLine = 1;
        break;
      default:
        perror(*argv);
        break;
    }
  }
  if (!options->e_pattern) {
    for (int count = 1; count < argc; ++count) {
      if (argv[count][0] != '-') {
        options->patterns[options->count_of_args++] = argv[count];
        count = argc;
      }
    }
  }
  if (!options->e_pattern) optind++;
}

void c_with_l(opt options, int counterMatches) {
  if (options.l_namesOfFiles) {
    printf("%d\n", counterMatches > 1 ? 1 : counterMatches);
  } else {
    printf("%d\n", counterMatches);
  }
}

void output(char* argv[], opt options, int mof) {
  char buffer[5000];
  regex_t regex;
  regmatch_t start;
  int counterLines = 0;
  int counterMatches = 0;
  int checkingEndFile = 0;
  int flagI = options.i_ignorRegistr ? REG_ICASE : REG_EXTENDED;
  FILE* file = fopen(argv[optind], "r");
  if (file == NULL) {
    perror(*argv);
    return;
  }
  while (fgets(buffer, 5000, file) != NULL) {
    counterLines++;
    int match = 0;
    for (int index = 0; index < options.count_of_args; ++index) {
      regcomp(&regex, options.patterns[index], flagI);
      match = regexec(&regex, buffer, 1, &start, 0) == 0 ? 1 : 0;
      regfree(&regex);
      if (options.v_withoutPattern) match = !match;
      if (match && !options.l_namesOfFiles && !options.c_countPattens && mof)
        printf("%s:", argv[optind]);
      if (match) counterMatches++;
      if (match && options.n_numOfLine && !options.c_countPattens &&
          !options.l_namesOfFiles)
        printf("%d:", counterLines);
      if (match && !options.c_countPattens && !options.l_namesOfFiles) {
        printf("%s", buffer);
        checkingEndFile = feof(file) ? 1 : 0;
      }
      if (match) break;
    }
  }
  if (options.c_countPattens && mof) printf("%s:", argv[optind]);
  if (options.c_countPattens) c_with_l(options, counterMatches);
  if (counterMatches && options.l_namesOfFiles) printf("%s\n", argv[optind]);
  if (counterMatches && checkingEndFile) printf("\n");
  fclose(file);
}

void processing(int argc, char* argv[]) {
  opt options = {0};
  parser(argc, argv, &options);
  int mof = optind + 1 < argc;
  while (optind < argc) {
    output(argv, options, mof);
    optind++;
  }
}