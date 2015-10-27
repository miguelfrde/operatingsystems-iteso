#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
 

#define ITERS 10

char *country[3] = {"Peru", "Bolivia", "Colombia"};
sem_t *sem;

void process(int i) {
  for (int k = 0; k < ITERS; k++) {
    // TODO Enter critical section
    printf("Entra %s ", country[i]);
    fflush(stdout);
    sleep(rand() % 3);
    printf("- Sale %s\n", country[i]);
    // TODO Exit critical section
    sleep(rand() % 3);
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  int pid, status;

  srand(getpid());

  //TODO initialize message stuff

  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid == 0) {
      process(i);
    }
  }

  for (int i = 0; i < 3; i++) {
    pid = wait(&status);
  }

  return 0;
}
