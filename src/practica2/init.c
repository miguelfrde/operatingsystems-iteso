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
  int child[6]; //array that holds the pid of child processes
  // Creates 6 procesess and replaces them with getty
  for (i = 0; i < 6; ++i) {
    pid = fork();
    if (pid == 0) {
      execlp("xterm", "xterm -e", "./getty", NULL);
    }
  }

  for(i = 0; i < 6; ++i) {
    child[i]=wait(&status);
    if(WIFEXITED(status)){
      if(WEXITSTATUS(status) == SIGINT){
          printf("shutdown recieved");
          break;
      }
      else{
        child[i] = fork();
        if(child[i] == 0) {
          execlp("xterm", "xterm -e", "./getty", NULL);
        }
      }

    }
  }

  for(i = 0; i < 6; ++i){
    kill(child[i], SIGINT);
  }
  return 0;

}
