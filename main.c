#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "built_ins.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char* readFromConsole(void) {
  int bufsize = 1024;
  int position = 0;
  char* buffer = (char*)malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "%sShell%s: allocation error\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  char cwd[1024], username[1024];
  getlogin_r(username, 1024);
  getcwd(cwd, sizeof(cwd));

  fprintf(stdout, "%s%s@%s$%s ", ANSI_COLOR_YELLOW, username, ANSI_COLOR_RESET, cwd);
  while (1) {
    c = getchar();

    // Kad bude kraj unosa stavljamo \0 i vracamo string
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // Povecavanje buffera
    if (position >= bufsize) {
      bufsize += 1024;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "%sShell%s: allocation error\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
    }
  }
}

char** parseArguments(char* line) {
  //Razdvajanje
  int bufsize = 64, position = 0;
  char** arguments = (char**)malloc(bufsize * sizeof(char*));
  char* buffer;

  if (!arguments) {
    fprintf(stderr, "%sShell%s: allocation error\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    exit(EXIT_FAILURE);
  }

  while((buffer = strsep(&line, " ")) != NULL) {
    arguments[position] = buffer;
    position++;

    if (position >= bufsize) {
      bufsize += bufsize;
      arguments = (char**)realloc(arguments, bufsize);
      if (!arguments) {
        fprintf(stderr, "%sShell%s: allocation error\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        exit(EXIT_FAILURE);
      }
    }
  }
  arguments[position] = NULL;
  return arguments;
}

int execute(char** args) {
  //provjera aliasa

  int status = 0;
  //Kao prvo gleda za built ins pa onda ostatak
  if ((status = exec_built_in(args)) == -1) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
      // dijete
      if (execvp(args[0], args) == -1) {
        perror("lsh");
      }
      exit(EXIT_FAILURE);
    } else if (pid < 0) {
      // neuspjel fork
      perror("lsh");
    } else {
      // roditelj
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return status;
  }
}

int main(void) {
  //Varijable za obradu podataka
  char* input;
  char** args;
  int status, i;

  //Glavna petlja koja ce se vrtit
  do {
    input = readFromConsole();
    if (input[0] != '\0') {
      args = parseArguments(input);
      status = execute(args);

      free(args);
    }
    free(input);
  } while (!status);

  return 0;
}
