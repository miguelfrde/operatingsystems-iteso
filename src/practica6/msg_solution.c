#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/msg.h>
#include "message.h"

#define ITERS 4


char *country[3] = {"Peru", "Bolivia", "Colombia"};

int mailbox;
Message *send, *receive;

void process(int i) {
  for (int k = 0; k < ITERS; k++) {
    // Waits for a message with the type = i+1
    msgrcv(mailbox, receive, 100, 0, 0);
    printf("Entra %s ", country[i]);
    fflush(stdout);
    sleep(rand() % 3);
    printf("- Sale %s\n", country[i]);
    // Send a message to the mailbox to unlock the next process
    msgsnd(mailbox, send, 10, 0);
    sleep(rand() % 3);
  }
  exit(0);
}

int main(int argc, char* argv[]) {
  int pid, status;

  // Initialize the message structs
  send = malloc(sizeof(Message));
  receive = malloc(sizeof(Message));

  send->mType = 1;

  srand(getpid());

  //Creates a mailbox
  mailbox = msgget(1400, 0666 | IPC_CREAT );
  if (mailbox == -1){
    printf("Unable to create a Message Queue");
  }

  //Send the first message to unblock the first process
  msgsnd(mailbox, send, 10, 0);

  for (int i = 0; i < 3; i++) {
    pid = fork();
    if (pid == 0) {
      process(i);
    }
  }

  for (int i = 0; i < 3; i++) {
    pid = wait(&status);
  }

  // Cleans the mailbox
  msgctl(mailbox, IPC_RMID, 0);

  return 0;
}
