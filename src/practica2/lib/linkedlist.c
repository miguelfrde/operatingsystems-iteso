/**
 * Implementation of a double ended LinkedList that stores strings
 */

#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

/**
 * Initializes a new double ended linked list with no elements
 * Return:
 *   A LinkedList struct of size 0 (no elements)
 */
LinkedList ll_create() {
  LinkedList list;
  list.first = NULL;
  list.size = 0;
  return list;
}

/**
 * Inserts a new value in the linked list.
 * Args:
 *   list   The linkedlist where the value will be inserted
 *   value  The value to be inserted in the linkedlist
 */
void ll_insert(LinkedList* list, char* value) {
  // Allocate memory for the new node to insert
  LinkedListNode* node = (LinkedListNode*)calloc(1, sizeof(LinkedListNode));
  node->next = NULL;
  node->value = (char*)calloc(1, sizeof(value));
  strcpy(node->value, value);

  // If the linked list is empty, insert it in the first position, if not
  // append it to the list.
  if (list->first) {
    list->last->next = node;
  } else {
    list->first = node;
  }
  list->last = node;

  list->size++;
}

/**
 * Removes all the elements in the linkedlist by freeing them
 * Args:
 *   list   The linkedlist to be emptied
 */
void ll_clear(LinkedList* list) {
  if (list->first == NULL) {
    return;
  }

  LinkedListNode* nextNode;
  for (LinkedListNode* node = list->first; node; node = nextNode) {
    nextNode = node->next;
    free(node->value);
    free(node);
  }
}

/**
 * Remove and return the value of the first element of the linked list
 */
char* ll_pop_first(LinkedList* list) {
  if (list->first == NULL) {
    return NULL;
  }
  char* first_value = (char*)calloc(1, sizeof(list->first->value));
  LinkedListNode* next = list->first->next;
  strcpy(first_value, list->first->value);
  free(list->first->value);
  free(list->first);
  list->first = next;
  list->size--;
  return first_value;
}
