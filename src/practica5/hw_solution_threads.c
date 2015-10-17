#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Macro for the machine instruction xchg
#define atomic_xchg(A,B) __asm__ __volatile__( \
    " lock xchg %1,%0 ;\n" \
    : "=ir" (A) \
    : "m" (B), "ir" (A) \
    );
#define ITERS 10


char *country[3] = {"Peru", "Bolivia", "Colombia"};
int g = 0;


void *thread1(void *arg) {
  int *mynum = (int *)arg;
  int i = *mynum;
  int l;

  for (int k = 0; k < ITERS; k++) {
    l = 1;

    do {
      atomic_xchg(l, g);
    } while (l != 0);

    // Begin critical section
    printf("Entra %s", country[i]);
    fflush(stdout);
    sleep(rand() % 3);
    printf(" - Sale %s\n", country[i]);
    // End critical section

    g = 0;
    l = 1;

    // Random wait outside of the critical section
    sleep(rand() % 3);
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  pthread_t tid[3];
  int args[3];
  void *thread_result;

  srand(getpid());

  // Create the threads
  for (int i = 0; i < 3; i++) {
    args[i] = i;
     pthread_create(&tid[i], NULL, thread1, (void *) &args[i]);
  }

  // Wait for the threads to finish
  for (int i = 0; i < 3; i++) {
    pthread_join(tid[i], &thread_result);
  }

  return 0;
}
