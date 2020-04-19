#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

/* A linked list node */
struct Node
{
    char *line;
    bool valid;
    int byteOffset;
    int lineNumber;
    bool aParameterMatch;
    struct Node *next;
};

void AddToEndOfLinkedList(struct Node** head_ref, char *new_line, bool valid, int byteOffset, int lineNumber, bool aParameterMatch);
void FreeLinkedList(struct Node** head_ref);

#endif