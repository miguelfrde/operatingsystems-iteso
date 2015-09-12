#include <stdio.h>
#include <unistd.h>

int main(){

	int pid;
	int i;
	int status; //may not be needed

	//creates 6 procesess and replaces them with getty
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