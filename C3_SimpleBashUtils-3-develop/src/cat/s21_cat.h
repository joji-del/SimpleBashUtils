#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Flags;

int cat(FILE *f, const Flags *flags);
void print_file(char *name, Flags *flag);
int parse(int argc, char *argv[], Flags *flag, int *end_ind_flag);

#endif