#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Options opts = {0};
  int fl = 0;

  fl = parse_arguments(argc, argv, &opts);

  if (fl == 0 && opts.pattern_count == 0) {
    fprintf(stderr, "s21_grep: no pattern provided\n");
    free_options(&opts);
    fl = 1;
  }

  if (fl == 0 && opts.file_count == 0) {
    opts.files[opts.file_count++] = "-";
  }

  if (fl == 0) {
    grep_files(&opts, &fl);
  }

  free_options(&opts);

  return fl;
}

int parse_arguments(int argc, char *argv[], Options *opts) {
  int fl = 0;
  int opt;
  int e_opt_used = 0;

  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1 && fl == 0) {
    if (opt == 'e') {
      opts->e_flag = 1;
      add_pattern(opts, optarg, &fl);
      e_opt_used = 1;
    } else if (opt == 'f') {
      opts->f_flag = 1;
      add_patterns_from_file(opts, optarg, &fl);
      e_opt_used = 1;
    } else if (opt == '?') {
      fprintf(stderr, "s21_grep: invalid option -- %c\n", optopt);
      fl = 1;
    } else {
      if (opt == 'i') opts->i_flag = 1;
      if (opt == 'v') opts->v_flag = 1;
      if (opt == 'c') opts->c_flag = 1;
      if (opt == 'l') opts->l_flag = 1;
      if (opt == 'n') opts->n_flag = 1;
      if (opt == 'h') opts->h_flag = 1;
      if (opt == 's') opts->s_flag = 1;
      if (opt == 'o') opts->o_flag = 1;
    }
  }
  for (int i = optind; i < argc && fl == 0; i++) {
    if (!e_opt_used && opts->pattern_count == 0) {
      add_pattern(opts, argv[i], &fl);
      e_opt_used = 1;
    } else if (opts->file_count < 100) {
      opts->files[opts->file_count++] = argv[i];
    } else {
      fprintf(stderr, "s21_grep: too many files\n");
      fl = 1;
    }
  }

  return fl;
}

void add_pattern(Options *opts, const char *pattern, int *fl) {
  if (opts->pattern_count < 100) {
    opts->patterns[opts->pattern_count++] = strdup(pattern);
  } else {
    fprintf(stderr, "s21_grep: too many patterns\n");
    *fl = 1;
  }
}

void add_patterns_from_file(Options *opts, const char *filename, int *fl) {
  FILE *file = fopen(filename, "r");

  if (!file) {
    if (!opts->s_flag) {
      fprintf(stderr, "s21_grep: cannot open file %s\n", filename);
    }
    *fl = 1;
  } else {
    char line[1024];
    while (fgets(line, sizeof(line), file) && *fl == 0) {
      line[strcspn(line, "\n")] = '\0';
      if (strlen(line) > 0) {
        add_pattern(opts, line, fl);
      }
    }
    fclose(file);
  }
}

void grep_files(Options *opts, int *fl) {
  int multiple_files = opts->file_count > 1;

  for (int i = 0; i < opts->file_count && *fl == 0; i++) {
    grep_file(opts->files[i], opts, multiple_files, fl);

    if (opts->l_flag && *fl == 1) {
      *fl = 0;
    }
  }
}

void grep_file(const char *filename, Options *opts, int multiple_files,
               int *fl) {
  FILE *file = NULL;

  if (strcmp(filename, "-") == 0) {
    *fl = 1;
  } else {
    file = fopen(filename, "r");
  }
  if (!file) {
    if (!opts->s_flag) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    }
    *fl = 1;
  } else {
    regex_t regexes[100];
    int cflags = REG_EXTENDED;
    compile_patterns(opts, regexes, &cflags, fl);

    if (*fl == 0) {
      process_file(file, filename, opts, regexes, multiple_files, fl);
    }

    free_patterns(regexes, opts->pattern_count);

    if (file != stdin) {
      fclose(file);
    }
  }
}

void compile_patterns(Options *opts, regex_t *regexes, int *cflags, int *fl) {
  if (opts->i_flag) {
    *cflags |= REG_ICASE;
  }

  for (int i = 0; i < opts->pattern_count && *fl == 0; i++) {
    int reti = regcomp(&regexes[i], opts->patterns[i], *cflags);
    if (reti != 0) {
      if (!opts->s_flag) {
        char msgbuf[100];
        regerror(reti, &regexes[i], msgbuf, sizeof(msgbuf));
        fprintf(stderr, "s21_grep: regex compilation failed: %s\n", msgbuf);
      }
      free_patterns(regexes, i);
      *fl = 1;
    }
  }
}

void process_file(FILE *file, const char *filename, Options *opts,
                  regex_t *regexes, int multiple_files, int *fl) {
  char line[1024];
  int line_number = 0;
  int match_found = 0;
  int match_found_l = 0;

  while (fgets(line, sizeof(line), file) && *fl == 0) {
    line_number++;
    int local_match = 0;

    process_line(line, line_number, filename, opts, regexes, multiple_files,
                 &local_match, fl);

    if (local_match) {
      match_found++;
    }

    if (opts->l_flag && local_match && match_found_l != 1 && match_found != 1) {
      match_found_l = 1;
    }
  }

  if (opts->c_flag) {
    if (!opts->h_flag && multiple_files) {
      printf("%s:", filename);
    }
    printf("%d\n", match_found);
  }
  if (opts->l_flag && match_found > 0) {
    printf("%s\n", filename);
  }
}

void print_flags_others(Options *opts, regex_t *regexes, const char *filename,
                        int multiple_files, int *h, int *y, const char *line,
                        int line_number) {
  if (opts->o_flag && !opts->v_flag) {
    for (int i = 0; i < opts->pattern_count; i++) {
      regex_t *regex = &regexes[i];
      const char *p = line;
      regmatch_t pmatch[1];
      while (regexec(regex, p, 1, pmatch, 0) == 0) {
        int start = pmatch[0].rm_so + (p - line);
        int end = pmatch[0].rm_eo + (p - line);

        if (!opts->h_flag && multiple_files && *h != 1) {
          printf("%s:", filename);
          *h = 0;
        }

        if (opts->n_flag && *y != 1) {
          printf("%d:", line_number);
        }

        printf("%.*s\n", end - start, line + start);

        p += pmatch[0].rm_eo;
        if (pmatch[0].rm_so == pmatch[0].rm_eo) {
          p++;
        }
        *h = 0;
        *y = 0;
      }
    }
  } else {
    printf("%s", line);
    if (line[strlen(line) - 1] != '\n') {
      printf("\n");
    }
  }
}

void process_line(const char *line, int line_number, const char *filename,
                  Options *opts, regex_t *regexes, int multiple_files,
                  int *match_count, int *fl) {
  int found = 0;

  for (int i = 0; i < opts->pattern_count && found == 0; i++) {
    regex_t *regex = &regexes[i];
    if (regexec(regex, line, 0, NULL, 0) == 0) {
      found = 1;
    }
  }

  if (opts->v_flag) {
    found = !found;
  }
  int h = 0;
  int y = 0;
  if (found) {
    (*match_count)++;
    if (opts->l_flag) {
      *fl = 1;
    } else if (!opts->c_flag) {
      if (!opts->h_flag && multiple_files) {
        printf("%s:", filename);
        h = 1;
      }
      if (opts->n_flag) {
        printf("%d:", line_number);
        y = 1;
      }
      print_flags_others(opts, regexes, filename, multiple_files, &h, &y, line,
                         line_number);
    }
  }
}

void free_patterns(regex_t *regexes, int count) {
  for (int i = 0; i < count; i++) {
    regfree(&regexes[i]);
  }
}

void free_options(Options *opts) {
  for (int i = 0; i < opts->pattern_count; i++) {
    free(opts->patterns[i]);
  }
}