#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define atomic_xchg(A,B) __asm__ __volatile__( \
    " lock xchg %1,%0 ;\n" \
    : "=ir" (A) \
    : "m" (B), "ir" (A) \
    );

#define ITERS 10

char *country[3] = {"Peru","Bolivia","Colombia"};
int *g;

void process(int i) {
  int l;

  for (int k = 0; k < ITERS; k++) {
    l = 1;

    do {
      atomic_xchg(l,*g);
    } while (l != 0);

    // Begin critical section
    printf("Entra %s", country[i]);
    fflush(stdout);
    sleep(rand() % 3);
    printf(" - Sale %s\n", country[i]);
    // End critical section

    l = 1;
    *g = 0;

    // Random wait outside of the critical section
    sleep(rand() % 3);
  }

  exit(0);
}

int main(int argc, char* argv[]) {
  int pid;
  int status;
  int shmid;

  // Request the shared memory
  shmid = shmget(0x1234, sizeof(g), 0666 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error en la memoria compartida\n");
    exit(1);
  }

  // Connect the variable to the shared memory
  g = shmat(shmid, NULL, 0);
  if (g == NULL) {
    perror("Error en el shmat\n");
    exit(2);
  }

  *g = 0;
  srand(getpid());

  for  (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid==0) {
      process(i);
    }
  }

  for (int i = 0; i < 3; i++) {
    pid = wait(&status);
  }

  // Delete the shared memory
  shmdt(g);

  return 0;
}
