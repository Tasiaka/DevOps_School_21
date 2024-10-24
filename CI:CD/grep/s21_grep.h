#ifndef MAIN_H
#define MAIN_H
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

#define LEN 1024

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int f;
} options;

void processing(const int *count_files, char **argv, int var,
                const options *flags, char *pattern);
void mini_output(const int *count_files, char **argv, char *line,
                 int number_str, int i_of_argv, char *pattern,
                 const options *flag);
void f(const char *fileName, char *pattern, const options *flags);
void c(const int *count_files, const char *name_file, int count_for_c,
       const options *flags);
void o(char *line, char *pattern, const options *flags);
void parser(int argc, char **argv, options *flags, char *e_patterns,
            char *f_pattern);
#endif

// -e: указывает, что следующий аргумент является регулярным выражением для
// поиска. -i: игнорирует регистр при поиске. -v: выводит только строки, которые
// не содержат совпадений. -c: выводит только количество строк, содержащих
// совпадения. -l: выводит только имена файлов, содержащих совпадения. -n:
// выводит номера строк, содержащих совпадения. -h: не выводит имена файлов при
// поиске в нескольких файлах. -s: подавляет сообщения об ошибках.
//-f:
// указывает, что следующий аргумент содержит файл с паттернами поиска. -o:
// выводит только сами совпадения, без всей строки.
