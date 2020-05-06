#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef struct Node {
  char *line;
  bool is_match;
  int match_offset;
  int line_number;
  bool reported;
  struct Node *next;
} Node;

int AddToEndOfLinkedList(struct Node **head_ref, char *new_line, bool is_match, int byte_offset, int line_number);
void FreeLinkedList(struct Node **head_ref);

#endif