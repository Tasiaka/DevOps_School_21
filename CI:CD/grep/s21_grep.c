#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc >= 2) {
    int testArg = 1;
    options flags = {0};
    char e_pattern[LEN] = "";
    char f_pattern[LEN] = "";
    parser(argc, argv, &flags, e_pattern, f_pattern);
    if (flags.e && argc == 2) {
      testArg = 0;
    }
    if ((flags.e || flags.f) && argc < 4) {
      testArg = 0;
    }
    if ((!flags.e && !flags.f) && argc <= 3) {
      testArg = 0;
    }
    if (testArg) {
      char pattern[LEN] = "";
      int var = 0;
      int count_files = 0;
      if (flags.e || flags.f) {
        count_files = argc - optind;
      } else {
        strcat(pattern, argv[optind]);
        var = 1;
        count_files = argc - optind - 1;
      }
      strcat(pattern, e_pattern);
      strcat(pattern, f_pattern);
      processing(&count_files, argv, var, &flags, pattern);
    }
    return 0;
  }
}

void processing(const int *count_files, char **argv, int var,
                const options *flags, char *pattern) {
  for (int i = 0; i < *count_files; i++) {
    FILE *fp;
    regex_t reg;
    if (flags->i)
      regcomp(&reg, pattern, REG_EXTENDED | REG_ICASE);
    else
      regcomp(&reg, pattern, REG_EXTENDED);
    if ((fp = fopen(argv[optind + var + i], "r")) != NULL) {
      int count_for_c = 0;
      int number_str = 1;
      int L = 0;
      char line[LEN] = {0};
      while (fgets(line, LEN, fp) != NULL) {
        if (regexec(&reg, line, 0, NULL, 0) == 0 && flags->v == 0) {
          if (flags->c && flags->l) {
            count_for_c = 1;
          } else if (flags->c && !flags->l) {
            count_for_c++;
          }
          if (!flags->l) {
            mini_output(count_files, argv, line, number_str, optind + i + var,
                        pattern, flags);
          } else
            L = 1;
        } else if (regexec(&reg, line, 0, NULL, 0) != 0 && flags->v) {
          if (flags->c && flags->l) {
            count_for_c = 1;
          } else if (flags->c && !flags->l) {
            count_for_c++;
          }
          if (!flags->l) {
            mini_output(count_files, argv, line, number_str, optind + i + var,
                        pattern, flags);
          } else
            L = 1;
        }
        number_str++;
      }
      if (flags->c) {
        c(count_files, argv[optind + var + i], count_for_c, flags);
      }
      if (flags->l && L) {
        printf("%s\n", argv[optind + var + i]);
      }
      fclose(fp);
    }
    // else
    //   printf("n/a");
    regfree(&reg);
  }
  //  if (flags->v && flags->o)  printf("\n");
}

void mini_output(const int *count_files, char **argv, char *line,
                 int number_str, int i_of_argv, char *pattern,
                 const options *flags) {
  if (flags->v == 0 || flags->o == 0) {
    if (flags->c == 0) {
      if (*count_files > 1 && flags->h == 0) {
        printf("%s:", argv[i_of_argv]);
      }
      if (flags->n) printf("%d:", number_str);

      if (flags->o) {
        o(line, pattern, flags);  ///////////////////
      } else {
        fputs(line, stdout);
      }
      if (*count_files > 1) {
        if (line[strlen(line)] == '\0' && line[strlen(line) - 1] != '\n') {
          printf("\n");
        }
      }
    }
  }
}

void c(const int *count_files, const char *name_file, int count_for_c,
       const options *flags) {
  if (flags->l == 0) {
    if (*count_files > 1 && flags->h == 0) {
      printf("%s:%d\n", name_file, count_for_c);
    } else {
      printf("%d\n", count_for_c);
    }
  }
}

void f(const char *fileName, char *pattern, const options *flags) {
  FILE *f;
  if ((f = fopen(fileName, "r")) == NULL) {
    if (flags->s == 0) {
      perror("No FILE");
    }
  } else {
    char buffer[LEN];
    while (fgets(buffer, LEN, f) != NULL) {
      if (buffer[strlen(buffer)] == '\0' &&
          buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
      }
      if (pattern[0] == 0) {
        strcat(pattern, buffer);
      } else {
        strcat(pattern, "|");
        strcat(pattern, buffer);
      }
    }
    fclose(f);
  }
}

void o(char *line, char *pattern, const options *flags) {
  regex_t reg;
  regmatch_t match[4096];
  int status = 1;
  char *compare = line;

  if (flags->i) {
    if ((status = regcomp(&reg, pattern, REG_EXTENDED | REG_ICASE)) != 0) {
      printf("failed %d", status);  ///////////////////////
      regfree(&reg);
    }
  } else if ((status = regcomp(&reg, pattern, REG_EXTENDED)) != 0) {
    printf("failed %d", status);  ///////////////////////
    regfree(&reg);
  }

  if (status == 0 && flags->v == 0) {
    for (int i = 0; line[i] != '\0'; i++) {
      if (regexec(&reg, compare, 1, match, 0) != flags->v) break;
      printf("%.*s\n", (int)(match[0].rm_eo - match[0].rm_so),
             compare + match[0].rm_so);
      compare += match[0].rm_eo;
    }
  }
  regfree(&reg);
}

void parser(int argc, char **argv, options *flags, char *e_patterns,
            char *f_pattern) {
  char flag = 0;
  while ((flag = getopt(argc, argv, "e:ivclnhsof:?")) != -1) {
    if (flag == 'e') {
      flags->e = 1;
      if (e_patterns[0] == 0)
        strcat(e_patterns, optarg);
      else {
        strcat(e_patterns, "|");
        strcat(e_patterns, optarg);
      }
    } else if (flag == 'i')
      flags->i = 1;
    else if (flag == 'v' && flags->o == 0)
      flags->v = 1;
    else if (flag == 'v' && flags->o == 1)
      flags->v = 1;
    else if (flag == 'c')
      flags->c = 1;
    else if (flag == 'l')
      flags->l = 1;
    else if (flag == 'n')
      flags->n = 1;
    else if (flag == 'h')
      flags->h = 1;
    else if (flag == 's')
      flags->s = 1;
    else if (flag == 'o' && flags->v == 0)
      flags->o = 1;
    else if (flag == 'o' && flags->v == 1)
      flags->o = 1;
    else if (flag == 'f') {
      flags->f = 1;
      f(optarg, f_pattern, flags);
    } else if (flag == '?')
      printf("cat: illegal option -- %s\nusage: cat [-belnstuv] [file ...]",
             argv[optind - 1]);
  }
}
