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
      // If the status is the shutdown signal(SIGINT), stop the program
      // Otherwise create a new process
      if(WEXITSTATUS(status) == SIGINT){
          printf("shutdown recieved");
          break;
      }
      else{
        
        if(fork() == 0) {
          execlp("xterm", "xterm -e", "./getty", NULL);

        }
  
      }

    }
  }

  // Kill all the processes that belongs to the parent process group
  kill(0,SIGINT);  
  return 0;

}

