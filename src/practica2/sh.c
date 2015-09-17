
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lib/linkedlist.h"
#include "lib/program_state.h"
#include "lib/sh_input.h"

/* Datatypes */

typedef struct {
  char* command_name;
  void (*callback)(LinkedList args);
} CommandCallback;

typedef struct {
  LinkedList key;
  LinkedList value;
} EnvironmentMap;

/* Prototypes */

void execute_command(Command);
void change_env_vars_by_its_value(LinkedList*);
void execute_path_program(Command);
void callback_shutdown(LinkedList);
void callback_exit(LinkedList);
void callback_set(LinkedList);
void callback_echo(LinkedList);

/* Global variables */

const CommandCallback callbacks[4] = {
  {"shutdown", callback_shutdown},
  {"exit", callback_exit},
  {"set", callback_set},
  {"echo", callback_echo}
};

int main(int argc, char* argv[]) {
  char raw_input[MAX_BUFFER_LEN];
  Command command;
  bool read_successful;

  while (true) {
    read_input(raw_input);
    read_successful = parse_input(raw_input, &command);
    if (read_successful) {
      change_env_vars_by_its_value(&command.args);
      execute_command(command);
    }
  }

  return 0;
}

/**
 * Changes the environment variables in the arguments by its value.
 * The environment variables are of the type $ENV_VAR.
 */
void change_env_vars_by_its_value(LinkedList* args) {
  LinkedListNode* arg;
  for (arg = args->first; arg; arg = arg->next) {
    if (arg->value[0] == '$') {
      // Env var found, remove the dollar sign and find it in the env
      char* env_val = getenv(arg->value + 1);
      strcpy(arg->value, env_val);
    }
  }
}

/**
 * If the given command is from the ones we implemented and runs it.
 * If not, it runs the program by finding it first in the PATH.
 */
void execute_command(Command command) {
  for (int i = 0; i < sizeof(callbacks)/sizeof(CommandCallback); i++) {
    if (strcmp(command.name, callbacks[i].command_name) == 0) {
      callbacks[i].callback(command.args);
      return;
    }
  }

  execute_path_program(command);
}

/**
 * Finds a program in the path and executes it
 */
void execute_path_program(Command command) {
  // TODO: real implementation
  LinkedListNode* arg;
  printf("Execute %s with args: ", command.name);
  for (arg = command.args.first; arg; arg = arg->next) {
    printf("%s ", arg->value);
  }
  printf("\n");
}

/**
 * shutdown command, stops the shell process and notifies the
 * init process that it has to stop
 */
void callback_shutdown(LinkedList args) {
  exit(MESSAGE_SHUTDOWN_SHELL);
}

/**
 * exit command, stops the shell process and notifies the getty
 * parent process that it should stop
 */
void callback_exit(LinkedList args) {
  exit(MESSAGE_EXIT_SHELL);
}

/**
 * set command, sets an environment variable
 */
void callback_set(LinkedList args) {
  char* inputValues = args.first->value;
  char* existantPath;

  //Split the string into the variable and the valie
  char* splittedValues = strtok(inputValues , "=");

  //Sets the setVariable
  char* var = splittedValues;

  //Iterate to the next string of the split
  splittedValues = strtok (NULL, "=");

  //Sets the value of the setVariable
  char* value = splittedValues;
  //printf("%s %s \n", var, value);
  if(value[0]=='$'){
    value=value+1;
    splittedValues = strtok (value, ":");
    existantPath = splittedValues;
    splittedValues = strtok (NULL, ":");
    value = splittedValues;
    setenv(var, strcat(strcat(getenv(existantPath),":"),value), 1);
  }
  else{
    setenv(var, value, 1);
  }

  printf("New var %s: %s\n", var, getenv(var));
}

/**
 * echo command, prints the arguments given
 */
void callback_echo(LinkedList args) {
  LinkedListNode* arg;
  for (arg = args.first; arg; arg = arg->next) {
    printf("%s ", arg->value);
  }
  printf("\n");
}
