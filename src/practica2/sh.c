#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lib/linkedlist.h"
#include "lib/program_state.h"
#include "lib/sh_input.h"

/* Prototypes */

void read_input(char*);
void execute_command(Command);

/* Global variables */

const State state = RUNNING;

int main(int argc, char* argv[]) {
  char raw_input[MAX_BUFFER_LEN];
  Command command;
  bool read_successful;

  while (state != STOPPED) {
    read_input(raw_input);
    read_successful = parse_input(raw_input, &command);
    if (read_successful) {
      execute_command(command);
    }
  }

  return 0;
}

void execute_command(Command command) {
  LinkedListNode* arg;
  printf("Command: %s\n", command.name);
  for (arg = command.args.first; arg; arg = arg->next) {
    printf("Arg: %s\n", arg->value);
  }
}
