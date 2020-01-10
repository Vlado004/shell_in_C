#include <stdio.h>
#include <stdlib.h>

#include "built_ins.h"

char* readFromConsole(void) {
  int bufsize = 1024;
  int position = 0;
  char* buffer = (char*)malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += LSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char** parseArguments(char* line) {
  //Razdvajanje

  return NULL;
}

int execute(char** args) {
  //Izvrsavanje naredbi - nasi programi van patha?
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
  int status;

  //Glavna petlja koja ce se vrtit
  do {
    input = readFromConsole();//zasad cemo rec da daje bas komandu a ne kobasicu teksta
    puts(input);
    //parsiranje? - Razdvajanje + svasta nesto kasnije
    //Izvedba?

    //oslobadanje (treba vidit kako tocno readline radi)
    free(input);
  } while (status);

  return 0;
}
