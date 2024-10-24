#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc <= 1)
    printf("n/a");
  else {
    options flags = {0};
    int exit = parser(argc, argv, &flags);
    processing(argc, argv, &flags);
    if (exit)
      printf("cat: illegal option -- %s\nusage: cat [-belnstuv] [file ...]",
             argv[optind - 1]);
  }
  return 0;
}

int parser(int argc, char **argv, options *flags) {
  char flag = 0;
  int exit = 0;
  int option_index = 0;
  const struct option longname[] = {{"number", 0, 0, 'n'},
                                    {"number-nonblank", 0, 0, 'b'},
                                    {"squeeze-blank", 0, 0, 's'},
                                    {0, 0, 0, 0}};
  while ((flag = getopt_long(argc, argv, "benstvTE", longname,
                             &option_index)) != -1) {
    if (flag == 's')
      flags->s = 1;
    else if (flag == 'b')
      flags->b = 1;
    else if (flag == 'e') {
      flags->v = 1;
      flags->e = 1;
    } else if (flag == 'v')
      flags->v = 1;
    else if (flag == 'n')
      flags->n = 1;
    else if (flag == 't') {
      flags->t = 1;
      flags->v = 1;
    } else if (flag == 'E')
      flags->E = 1;
    else if (flag == 'T')
      flags->T = 1;
    else if (flag == '?') {
      exit = 1;
      break;
    }
  }
  if (flags->b) flags->n = 0;
  if (flags->E) flags->v = 0;
  if (flags->T) flags->v = 0;
  return exit;
}

void processing(int argc, char **argv, options *flags) {
  char letter;
  for (int i = optind; i < argc; i++) {
    int pr_letter = 10;
    int pr_pr_letter = 0;
    int index = 1;
    FILE *fp;
    fp = fopen(argv[i], "r");
    if (fp != NULL) {
      if (getc(fp) != EOF) {
        fseek(fp, 0, SEEK_SET);
        while ((letter = getc(fp)) != EOF) {
          s(letter, &pr_letter, &pr_pr_letter, flags, &index);
        }
      } else {
        printf("n/a");
      }
      fclose(fp);
    } else {
      printf("cat: %s: No such file or directory", argv[i]);
    }
  }
}
void s(char letter, int *pr_letter, int *pr_pr_letter, options *flags,
       int *index) {
  if (flags->s) {
    if (letter != '\n') {
      nb(letter, pr_letter, index, flags);
      *pr_pr_letter = *pr_letter;
      *pr_letter = letter;
    } else if (*pr_letter != '\n') {
      te(letter, flags);
      *pr_pr_letter = *pr_letter;
      *pr_letter = letter;
    } else if (*pr_pr_letter != '\n') {
      nb(letter, pr_letter, index, flags);
      *pr_pr_letter = *pr_letter;
      *pr_letter = letter;
    }
  } else {
    nb(letter, pr_letter, index, flags);
    *pr_letter = letter;
  }
}

void nb(char letter, const int *pr_letter, int *index, options *flags) {
  if (flags->b && *pr_letter == 10 && letter != '\n') {
    printf("%6d\t", *index);
    *index = *index + 1;
    te(letter, flags);
  } else if (flags->n && *pr_letter == 10) {
    printf("%6d\t", *index);
    te(letter, flags);
    *index = *index + 1;
  } else
    te(letter, flags);
}

void te(char letter, options *flags) {
  if (flags->e && letter == 10) {
    printf("$\n");
  } else if (flags->t && letter == 9) {
    printf("^I");
  } else
    v(letter, flags);
}

void v(char letter, options *flags) {
  if (flags->v) {
    if ((letter >= 0 && letter < 9) || (letter >= 11 && letter <= 31)) {
      printf("^%c", letter + 64);
    } else if (letter == 127)
      printf("^?");
    else
      printf("%c", letter);
  } else {
    printf("%c", letter);
  }
}
