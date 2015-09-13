#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* fileRoot = "../../Hello.txt";

void cleanInput(char *str) {
  int last_index = strlen(str) - 1;
  if (str[last_index] == '\n' || str[last_index] == '\r') {
    str[last_index] = '\0';
  }
}

int main(){
  //User and Password wrote in console
  char user[64];
  char password[64];

  //Process ID var
  int pid;
  char spcpu[5];

  //Declaring variables to split each line of our file with a Comma

  char lineIterator[64]; //Line iterator
  char *cleanLine;       //Line after the clean up process
  char *credentials[2];  //Credentials in the file
  int i = 0;             //Iterator index

  //Prompt for credentials
  printf("User:");
  scanf("%s", user);
  printf("Password:");
  scanf("%s", password);

  //Declaring the root of our file in Read-Only Mode
  FILE *fp;
  fp = fopen(fileRoot,"r");
	
  //If the file is empty, returns an error
  if( fp == NULL ){
    perror("Error while opening the file.\n");
    return -1;
  }

    /* While the loop has not reached the end of the file
     The loop will iterate each line of the file until it founds a match with the
     Values given by the users*/
    while(!feof(fp)){
      //Read a line of the file and stores it on SingleLine var
      fgets(lineIterator,150,fp);
	
      //Removes trash characters	
      cleanInput(lineIterator);
      
      //When the line is clean, splits it with the comma	
      cleanLine = strtok(lineIterator, ",");

      //Split process (Credits: StackOverflow) 
      while(cleanLine!=NULL){
        credentials[i++] = cleanLine;
        cleanLine = strtok (NULL, ",");
      }
      i=0;
      //If user wrote in console is equals to user in the file
      if(strcmp(user, credentials[0])==0){	
        //If password wrote in console is equals to password in the file 
        if(strcmp(password, credentials[1])==-13){
          puts("Successful login");
          printf("doing re-image...");	
          break;
        }
      }
    }
    //Close the file
    fclose(fp);  	

    //Create a child process
    pid = fork();

    if(pid == 0){
      //Replace with SH process
      //execlp("nice","nice","--adjustment=0","./cpuyes",spcpu,0);
    }
    else{
      wait(NULL);	
      //TODO re-prompt
    }

  return 0;
}
