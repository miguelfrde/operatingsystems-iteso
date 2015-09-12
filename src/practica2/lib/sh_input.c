#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "sh_input.h"

/**
 * Gets the user input from the stdin
 * Args:
 *   raw_input    Where the read input will be saved
 */
void read_input(char* raw_input) {
  printf("$ ");
  fgets(raw_input, MAX_BUFFER_LEN, stdin);
}

/**
 * Auxiliar function to insert a substring of a string in the linkedlist.
 * The substring is the stringi n range [start, end]
 */
void insert_substring(LinkedList* list, char* string, int start, int end) {
  char substring[MAX_BUFFER_LEN];
  strncpy(substring, string + start, end - start + 1);
  substring[end - start + 1] = '\0';
  printf("> %s\n", substring);
  ll_insert(list, substring);
}

/**
 * Auxiliar function to handle the open-close quotes. Extracted like this
 * to avoid repeating basically the same code for single quotes and double
 * quotes
 */
void handle_quote(LinkedList* list, bool* expecting_this, bool* expecting_other,
                  char* raw_input, int* begin, int end) {
  if (*expecting_other) {
    return;
  }
  if (*expecting_this) {
    insert_substring(list, raw_input, *begin, end);
    *begin = end + 1;
    *expecting_this = false;
  } else {
    *expecting_this = true;
    *begin = end;
  }
}

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
bool parse_input(char* raw_input, Command* command) {
  int input_length = strlen(raw_input);
  int token_begins = 0;
  bool expect_single_quote = false, expect_double_quote = false;

  if (raw_input == NULL || input_length == 0) {
    return false;
  }

  // Get all the strings separated by spaces, ignore spaces
  // when the strings are enclosed in "" or ''.
  command->args = ll_create();
  for (int i = 0; i < input_length; i++) {
    char c = raw_input[i];
    switch (c) {
      case '\'':
        handle_quote(&command->args, &expect_single_quote, &expect_double_quote,
                     raw_input, &token_begins, i);
        break;
      case '"':
        handle_quote(&command->args, &expect_double_quote, &expect_single_quote,
                     raw_input, &token_begins, i);
        break;
      case ' ':
        if (expect_double_quote || expect_single_quote) continue;
        if (token_begins < i - 1) {
          insert_substring(&command->args, raw_input, token_begins, i - 1);
          token_begins = i + 1;
        }
        break;
    }
  }

  // Add the rest of the input
  if (token_begins != input_length) {
    insert_substring(&command->args, raw_input, token_begins, input_length - 1);
  }

  // Make the first save arg the command name
  strcpy(command->name, ll_pop_first(&command->args));

  return true;
}
