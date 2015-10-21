#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "semaphore.h"

#define ITERS 10

char *country[3] = {"Peru", "Bolivia", "Colombia"};
Semaphore *sem;

void process(int i) {
  for (int k = 0; k < ITERS; k++) {
    waitsem(sem);
    printf("Entra %s ", country[i]);
    fflush(stdout);
    sleep(rand() % 3);
    printf("- Sale %s\n", country[i]);
    signalsem(sem);
    sleep(rand() % 3);
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  int shmid, pid, status;

  // Request shared memory
  shmid = shmget(0x1234, sizeof(sem), 0666 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error en la memoria compartida\n");
    exit(1);
  }

  // Allocate and create semaphore
  sem = shmat(shmid, NULL, 0);
  if (sem == NULL) {
    perror("Error en la memoria compartida\n");
    exit(2);
  }
  initsem(sem, 1);

  srand(getpid());

  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid == 0) {
      process(i);
    }
  }

  for (int i = 0; i < 3; i++) {
    pid = wait(&status);
  }

  shmdt(sem);

  return 0;
}
