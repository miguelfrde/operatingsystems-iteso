#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "semaphore.h"

pid_t dequeue(Queue *queue) {
  if (queue->size == 0) {
    fprintf(stderr, "Semaphore empty blocking queue, cannot dequeue\n");
    exit(1);
  }
  pid_t value = queue->items[queue->head];
  queue->head++;
  queue->head = queue->head % QUEUE_SIZE;
  queue->size--;
  return value;
}

void enqueue(Queue *queue, pid_t value) {
  if (queue->size == QUEUE_SIZE) {
    fprintf(stderr, "Semaphore full blocking queue, cannot enqueue\n");
    exit(2);
  }
  queue->items[queue->tail] = value;
  queue->tail++;
  queue->tail = queue->tail % QUEUE_SIZE;
  queue->size++;
}

void waitsem(Semaphore *sem) {
  int l = 1;
  pid_t pid = getpid();

  do { atomic_xchg(l, sem->global); } while (l != 0);

  sem->counter--;
  if (sem->counter < 0) {
    enqueue(&sem->queue, pid);
    kill(pid, SIGSTOP);
  }

  sem->global = 0;
}

void signalsem(Semaphore *sem) {
  int l = 1;
  pid_t pid;

  do { atomic_xchg(l, sem->global); } while (l != 0);

  sem->counter++;
  if (sem->counter <= 0) {
     pid = dequeue(&sem->queue);
     kill(pid, SIGCONT);
  }

  sem->global = 0;
}

void initsem(Semaphore *sem, int counter) {
  sem->counter = counter;
  sem->global = 0;
  sem->queue.head = 0;
  sem->queue.tail = 0;
  sem->queue.size = 0;
}
