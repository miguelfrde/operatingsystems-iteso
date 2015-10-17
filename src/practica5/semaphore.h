#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

typedef struct Semaphore {
  int counter;
} Semaphore;

void waitsem(Semaphore *sem);

void signalsem(Semaphore *sem);

void initsem(Semaphore *sem, int counter);

#endif
