#include <stdio.h>
#include <unistd.h>

int main(){

	int pid;
	int i;
	int status;

	//crear los 6 procesos
	for (i = 0; i < 6; ++i){
		pid = fork();
		if(pid == 0)
			execlp("xterm", "xterm -e", "./dummygetty", NULL);
		
	}

	while(1){
		
		wait(&status);
		pid = fork();
		if(pid == 0)
			execlp("xterm", "xterm -e", "./dummygetty", NULL);
	}
	

}