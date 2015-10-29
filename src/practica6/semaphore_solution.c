#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>

#define ITERS 10

char *country[3] = {"Peru", "Bolivia", "Colombia"};
sem_t *sem;

void process(int i) {
  for (int k = 0; k < ITERS; k++) {
    sem_wait(sem);
    printf("Entra %s ", country[i]);
    fflush(stdout);
    sleep(rand() % 3);
    printf("- Sale %s\n", country[i]);
    sem_post(sem);
    sleep(rand() % 3);
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  int pid, status;

  srand(getpid());

  // Initializes semaphore
  sem = sem_open ("Sem", O_CREAT | O_EXCL, 0644, 1);

  sem_unlink("Sem");

  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid == 0) {
      process(i);
    }
  }

  for (int i = 0; i < 3; i++) {
    pid = wait(&status);
  }

  sem_close(sem);
  return 0;
}
