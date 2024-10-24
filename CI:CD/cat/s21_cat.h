#ifndef MAIN_H
#define MAIN_H
#include <getopt.h>
#include <stdio.h>
#include <string.h>
typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
} options;

void nb(char letter, const int *pr_letter, int *index, options *flags);
void te(char letter, options *flags);
void v(char letter, options *flags);
void s(char letter, int *pr_letter, int *pr_pr_letter, options *flags,
       int *index);
int parser(int argc, char **argv, options *flags);
void processing(int argc, char **argv, options *flags);

#endif