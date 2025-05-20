#ifndef S21_GREP_H
#define S21_GREP_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
  int f_flag;
  int o_flag;
  int pattern_count;
  char *patterns[100];
  int file_count;
  char *files[100];
} Options;

int parse_arguments(int argc, char *argv[], Options *opts);
void add_pattern(Options *opts, const char *pattern, int *fl);
void add_patterns_from_file(Options *opts, const char *filename, int *fl);
void grep_files(Options *opts, int *fl);
void grep_file(const char *filename, Options *opts, int multiple_files,
               int *fl);
void compile_patterns(Options *opts, regex_t *regexes, int *cflags, int *fl);
void process_file(FILE *file, const char *filename, Options *opts,
                  regex_t *regexes, int multiple_files, int *fl);
void process_line(const char *line, int line_number, const char *filename,
                  Options *opts, regex_t *regexes, int multiple_files,
                  int *match_count, int *fl);
void free_patterns(regex_t *regexes, int count);
void free_options(Options *opts);
void print_flags_others(Options *opts, regex_t *regexes, const char *filename,
                        int multiple_files, int *h, int *y, const char *line,
                        int line_number);

#endif