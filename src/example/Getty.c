#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void removeChar(char *str, char garbage) {

    char *src, *dst;
    int i = 0;
    for (src = dst = str; *src != '\0'; src++) {

        *dst = *src;
        if (*dst != garbage || *dst == '\n') dst++;
	else printf("^%c", *dst);
    }

    *dst = '\0';
}

int main(){
	char user[64];
	char password[64];
	printf("User:");
	scanf("%s", user);
	printf("Password:");
	scanf("%s", password);
	FILE *fp;
	fp = fopen("/home/alberto/Desktop/Hello.txt.txt","r");
	if( fp == NULL ){
     		perror("Error while opening the file.\n");
      	 	return -1;
   	}
	char singleLine[64];
	char *aux;
	char *credentials[2];
	int i = 0;
	while(!feof(fp)){
		fgets(singleLine,150,fp);
		removeChar(singleLine, '\0');		
		removeChar(singleLine, '\n');	
		aux = strtok(singleLine, ",");
		while(aux!=NULL){
		 	credentials[i++] = aux;
		 	aux = strtok (NULL, ",");
			//credentials[i++] = aux;
		 	//aux = strtok (NULL, ",");
		}
		printf("credentials 0: %s", credentials[0]);
		printf("credentials 1: %s", credentials[1]);		
		i=0;
		if(strcmp(user, credentials[0])==0){	
			if(strcmp(password, credentials[1])==-13){
				puts("Successful login");
				printf("do reimage");		
				break;
			}
		}	
		else{
			printf("Expected %s %s, received %s %s",credentials[0], credentials[1],user, password);
		}	
	}
	fclose(fp);
	return 0;
}
