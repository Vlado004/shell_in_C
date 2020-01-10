#ifndef BUILT_INS_H
#define BUILT_INS_H

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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
  return 0;
}

int change_dir(char** args) {
  int status = 0;

  return status;
}

int help(char** args) {
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
