#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "semaphore.h"

#define ITERS 10

char *country[3] = {"Peru","Bolivia","Colombia"};
Semaphore *sem;

void proceso(int i) {
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
	initsem(sem, 1);
}
