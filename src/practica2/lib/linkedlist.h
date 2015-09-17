#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define MAX_BUFFER_LEN 1000

/**
 * A double eneded LinkedList that stores strings
 */

typedef struct LinkedListNode {
  struct LinkedListNode* next;
  char value[MAX_BUFFER_LEN];
} LinkedListNode;

typedef struct LinkedList {
  LinkedListNode* first;
  LinkedListNode* last;
  int size;
} LinkedList;

LinkedList ll_create(void);
void ll_insert(LinkedList*, char*);
void ll_clear(LinkedList*);
char* ll_pop_first(LinkedList*);

#endif
