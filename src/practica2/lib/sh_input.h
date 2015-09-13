#include "linkedlist.h"

#define MAX_BUFFER_LEN 100

typedef struct {
  char name[MAX_BUFFER_LEN];
  LinkedList args;
} Command;

/**
 * Gets the user input from the stdin
 * Args:
 *   raw_input    Where the read input will be saved
 */
void read_input(char* raw_input);

/**
 * Parses a user's raw input into:
 *   Command name
 *   Command arguments
 * Args:
 *   raw_input    The command to parse
 *   command      Where the parsed command will be saved
 * Return:
 *   If the input is empty or NULL it returns false, which means
 *   that the parsing was not successful.
 */
bool parse_input(char*, Command*);
