#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Flags flag = {0};
  int fl = 1;
  int end_ind_flag = 1;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [-options] [file ...]\n", argv[0]);
    fl = 0;
  }

  if (argv[1][0] != '-') {
    end_ind_flag = 1;
    fl = 100;
  }

  if (fl != 100) {
    fl = parse(argc, argv, &flag, &end_ind_flag);
  }

  if (fl != 0) {
    if (argc > end_ind_flag) {
      for (int i = end_ind_flag; i < argc; i++) {
        print_file(argv[i], &flag);
      }
    } else {
      fprintf(stderr, "No files\n");
    }
  }

  return 0;
}

int cat(FILE *f, const Flags *flags) {
  int line_number = 1;
  int prev_char = '\n';
  int empty_line_streak = 0;
  int ch = fgetc(f);
  while (ch != EOF) {
    if (flags->s && ch == '\n' && prev_char == '\n' && empty_line_streak) {
    } else {
      if (flags->s && ch == '\n' && prev_char == '\n') {
        empty_line_streak = 1;
      } else {
        empty_line_streak = 0;
      }
      if ((flags->n && !flags->b && prev_char == '\n') ||
          (flags->b && prev_char == '\n' && ch != '\n')) {
        printf("%6d\t", line_number++);
      }
      if (flags->b && flags->e && prev_char == '\n' && ch == '\n') {
        printf("      \t");
      }
      if (ch == '\n') {
        if (flags->e) {
          printf("$");
        }
        putchar('\n');
      } else if (ch == '\t' && flags->t) {
        printf("^I");
      } else if (flags->v) {
        if ((ch < 32 && ch != '\n' && ch != '\t') || ch == 127) {
          putchar('^');
          putchar((ch == 127) ? '?' : ch + 64);
        } else if (ch > 127 && ch < 160) {
          printf("M-^");
          putchar(ch - 128 + 64);
        } else {
          putchar(ch);
        }
      } else {
        putchar(ch);
      }
    }
    prev_char = ch;
    ch = fgetc(f);
  }
  return 0;
}

void print_file(char *name, Flags *flag) {
  FILE *f = fopen(name, "r");
  if (f != NULL) {
    cat(f, flag);
    fclose(f);
  } else {
    fprintf(stderr, "cat: %s: No such file or directory\n", name);
  }
}

int parse(int argc, char *argv[], Flags *flag, int *end_ind_flag) {
  int rez;
  int fl = 1;
  struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                  {"number", no_argument, NULL, 'n'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {0, 0, 0, 0}};
  while ((rez = getopt_long(argc, argv, "beEnstTv", long_options, NULL)) !=
             -1 &&
         fl) {
    switch (rez) {
      case 'b':
        flag->b = 1;
        flag->n = 0;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 'n':
        if (!flag->b) flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case '?':
      default:
        fprintf(stderr, "Invalid option. Use [-benstv] [file ...]\n");
        fl = 0;
    }
  }
  *end_ind_flag = optind;
  return fl;
}
