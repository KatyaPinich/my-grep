#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

/* A linked list node */
typedef struct Node
{
    char *line;
    bool is_match;
    int match_offset;
    int line_number;
    bool aParameterMatch;
    bool reported;
    struct Node *next;
} Node;

void AddToEndOfLinkedList(struct Node** head_ref, char *new_line, bool valid, int byteOffset, int lineNumber,
        bool aParameterMatch);
void FreeLinkedList(struct Node** head_ref);

#endif