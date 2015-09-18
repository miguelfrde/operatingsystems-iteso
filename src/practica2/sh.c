#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>

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
void substitute_string(char*, char*, int, int);
bool is_valid_env_char(char);
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
    for (int i = 0; i < strlen(arg->value); i++) {
      if (arg->value[i] == '$') {
        char env_var[MAX_BUFFER_LEN];
        char* env_var_value;
        int j = i + 1;

        // Advance the index until the whole env var name is found
        while (is_valid_env_char(arg->value[j])) j++;
        strncpy(env_var, arg->value + i + 1, j - i - 1);
        env_var[j - i - 1] = '\0';

        // Get the env var value and substitute its $NAME by it
        env_var_value = getenv(env_var);
        substitute_string(arg->value, env_var_value, i, j - 1);
        if (env_var_value != NULL) {
          i += strlen(env_var_value);
        }
      }
    }
  }
}

/**
 * Returns true if an env var name can contain the given character
 */
bool is_valid_env_char(char c) {
  return isdigit(c) || isalpha(c) || c == '_';
}

/**
 * Replaces whatever is in the first string in the range [start, end]
 * with the contents of the second argument
 */
void substitute_string(char* original, char* to_insert, int start, int end) {
  char result[MAX_BUFFER_LEN];
  strncpy(result, original, start);
  result[start] = '\0';

  // Just ignore that string if it doesn't contain anything
  if (to_insert != NULL && strlen(to_insert) > 0) {
    strcat(result, to_insert);
  }

  strcat(result, original + end + 1);
  strcpy(original, result);
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
  LinkedListNode* arg;
  int i, pid, status;

  // Put all args in an array, to be able to pass them to exec
  char** args = (char**)calloc(command.args.size + 2, sizeof(char*));
  for (i = 1, arg = command.args.first; arg; arg = arg->next, i++) {
    args[i] = arg->value;
  }

  args[0] = command.name;

  // Execute the given program
  pid = fork();
  if (pid == 0) {
    execvp(command.name, args);
  } else {
    wait(&status);
  }

  free(args);
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

  //Split the string into the variable and the value
  char* splittedValues = strtok(inputValues , "=");
  char* var = splittedValues;
  char* value = strtok (NULL, "=");

  if (value[0] == '$') {
    value = value + 1;
    splittedValues = strtok (value, ":");
    existantPath = splittedValues;
    splittedValues = strtok (NULL, ":");
    value = splittedValues;
    setenv(var, strcat(strcat(getenv(existantPath), ":"), value), 1);
  }
  else {
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
