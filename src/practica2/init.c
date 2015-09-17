#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


#include "lib/program_state.h"

void launch_getty();

int main() {
  int status;

  // Creates 6 procesess and replaces them with getty
  for (int i = 0; i < 6; ++i) {
    launch_getty();
  }


  while(1) {
    wait(&status);
    if (WIFEXITED(status)) {
      // If the status is the shutdown signal(SIGINT), stop the program
      // Otherwise create a new process
      if (WEXITSTATUS(status) == SIGINT) {
          printf("shutdown recieved");
          break;
      } else {
        launch_getty();
      }
    }
  }

  // Kill all the processes that belongs to the parent process group
  kill(0,SIGINT);
  return 0;
}

// Launches a new getty process
void launch_getty() {
  if (fork() == 0) {
    execlp("xterm", "xterm -e", "./getty", NULL);
  }
}
