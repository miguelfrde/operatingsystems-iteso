#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#define QUEUE_SIZE 100
#define atomic_xchg(A,B) __asm__ __volatile__( \
    " lock xchg %1,%0 ;\n" \
    : "=ir" (A) \
    : "m" (B), "ir" (A) \
    );

typedef struct Queue {
  int head;
  int tail;
  int size;
  pid_t items[QUEUE_SIZE];
} Queue;

typedef struct Semaphore {
  int counter;
  int g;
  Queue queue;
} Semaphore;

void waitsem(Semaphore *sem);

void signalsem(Semaphore *sem);

void initsem(Semaphore *sem, int counter);

void destroysem(Semaphore *sem);

#endif
