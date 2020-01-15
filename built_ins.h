#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"

int change_dir(char **args);
int help(char **args);
int exit_shell(char **args);

char *built_in_str[] = {
  "cd",
  "help",
  "exit"
};

int (*built_in_func[]) (char **) = {
  &change_dir,
  &help,
  &exit_shell
};

int num_built_ins() {
  return sizeof(built_in_str) / sizeof(char *);
}

int exit_shell(char** args) {
  return 1;
}

int change_dir(char** args) {
  if (chdir(args[1])){
    fprintf(stderr, "%sERROR, directory '%s' not found\n", ANSI_COLOR_RED, args[1]);
  }
  return 0;
}

int help(char** args) {
  fprintf(stdout, "This is a work in progress shell made for a class.\nList of commands:\n");
  int size = num_built_ins(), i;
  for (i = 0; i < size; i++) {
    fprintf(stdout, "%s\n", built_in_str[i]);
  }
  return 0;
}

int exec_built_in(char** arguments) {
  int i, size = num_built_ins();

  for (i = 0; i < size; i++) {
    if (!strcmp(arguments[0], built_in_str[i]))
      return (*built_in_func[i])(arguments);
  }

  return -1;
}

#endif
