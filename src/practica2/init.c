#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
    return 0;
    if(status == MESSAGE_SHUTDOWN_SHELL){
      printf("recieved shutdown \n");
      exit(0);
    }
    else{
      pid = fork();
      if(pid == 0) {
        execlp("xterm", "xterm -e", "./getty", NULL);
      }
    }
    
  }

}
