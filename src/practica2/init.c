#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>  


#include "lib/program_state.h"

int main() {
  int pid;
  int i;
  int status;
  // Creates 6 procesess and replaces them with getty
  for (i = 0; i < 6; ++i) {
    pid = fork();
    if (pid == 0) {
      execlp("xterm", "xterm -e", "./getty", NULL);
    }
  }

  while(1) {
    wait(&status);
    if(WIFEXITED(status)){
      if(WEXITSTATUS(status) == SIGINT){
          printf("shutdown recieved");
          return 0;
      }
      else{
        pid = fork();
        if(pid == 0) {
          execlp("xterm", "xterm -e", "./getty", NULL);
        }
      }

    }
  }

}
