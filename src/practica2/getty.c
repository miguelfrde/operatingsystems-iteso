#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

#define PASS_BUFFER_LEN 150

/* Datatypes and prototypes */

typedef struct Credentials {
  char username[PASS_BUFFER_LEN];
  char password[PASS_BUFFER_LEN];
} Credentials;

void cleanInput(char*);
Credentials prompt_for_credentials();
bool authorized(Credentials);

// File where the valid credentials are stored
const char* fileRoot = "passwd";

int main(int argc, char* argv[]){
  Credentials credentials;
  int pid, status;

  credentials = prompt_for_credentials();

  if (authorized(credentials)) {
    pid = fork();
    if (pid == 0) {
      execlp("xterm", "xterm -e", "./sh", NULL);
    } else {
      wait(&status);
    }
  }

  return 0;
}

/**
 * Asks the user for the username and password
 */
Credentials prompt_for_credentials() {
  Credentials credentials;
  printf("User:");
  fgets(credentials.username, PASS_BUFFER_LEN, stdin);
  printf("Password:");
  fgets(credentials.password, PASS_BUFFER_LEN, stdin);
  cleanInput(credentials.password);
  cleanInput(credentials.username);
  return credentials;
}

/**
 * Remove breaklines from the string
 */
void cleanInput(char* str) {
  int last_index = strlen(str) - 1;
  if (str[last_index] == '\n' || str[last_index] == '\r') {
    str[last_index] = '\0';
  }
}

/**
 * Checks if the given credentials are valid. They are
 * valid if they are in the default file.
 */
bool authorized(Credentials given) {
  Credentials expected;
  char credentialsLine[PASS_BUFFER_LEN];
  char *token;
  FILE *fp;

  fp = fopen(fileRoot, "r");

  if (fp == NULL) {
    perror("Error while opening the file.\n");
    return -1;
  }

  // Iterate each line of the file until it founds a match with the
  // values given by the users
  while (fgets(credentialsLine, PASS_BUFFER_LEN, fp)) {
    cleanInput(credentialsLine);

    // Split by commas
    token = strtok(credentialsLine, ",");
    strcpy(expected.username, token);
    strcpy(expected.password, strtok(NULL, ","));

    // Check if both the username and the password are the same
    if (strcmp(given.username, expected.username) == 0 &&
        strcmp(given.password, expected.password) == 0) {
      printf("Successful login!\n");
      sleep(3);
      fclose(fp);
      return true;
    }
  }

  fclose(fp);

  printf("Authentication failed\n");
  sleep(3);

  return false;
}
