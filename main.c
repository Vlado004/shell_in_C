#include <stdio.h>
#include <stdlib.h>
#include "readline/readline.h"
//#include "readline/history.h"

#include "built_ins.h"

char* readFromConsole() {
  char* buffer readline("\n>>> ");

  return buffer;
}

char** parseArguments(char* line) {
  //Razdvajanje

}

int execute(char** args) {
  //Izvrsavanje naredbi - nasi programi van patha?
  //provjera aliasa

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
    //parsiranje? - Razdvajanje + svasta nesto kasnije
    //Izvedba?

    //oslobadanje (treba vidit kako tocno readline radi)
    free(input);
  } while (status);

  return 0;
}
