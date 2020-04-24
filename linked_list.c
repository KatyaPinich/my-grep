#include<stdio.h>
#include<stdlib.h>
#include "linked_list.h"

#define SUCCESS_CODE (0)
#define FAILED_CODE (-1)

struct Node* CreateNode(char *line, bool valid, int byteOffset, int lineNumber, bool aParameterMatch);

void AddToEndOfLinkedList(struct Node** head_ref, char *new_line, bool valid, int byteOffset, int lineNumber,
        bool aParameterMatch)
{
    struct Node* current = NULL;

    struct Node* new_node = CreateNode(new_line, valid, byteOffset, lineNumber, aParameterMatch);

    if (*head_ref == NULL)
    {
        *head_ref = new_node;
    }
    else
    {
        current = *head_ref;

        while (current->next != NULL)
            current = current->next;

        current->next = new_node;
    }
}

int Pop(struct Node** head_ref)
{
    struct Node * next_node = NULL;

    if (*head_ref == NULL)
        return FAILED_CODE;

    free((*head_ref)->line);

    next_node = (*head_ref)->next;
    free(*head_ref);
    *head_ref = next_node;

    return SUCCESS_CODE;
}

void FreeLinkedList(struct Node** head_ref)
{
    int return_code = SUCCESS_CODE;
    while (return_code != FAILED_CODE)
        return_code = Pop(head_ref);
}

struct Node* CreateNode(char *line, bool valid, int byteOffset, int lineNumber, bool aParameterMatch)
{
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    if (new_node == NULL)
    {
        printf("Failed to create a new node in linked list.");
        exit(1);
    }

    new_node->line = line;
    new_node->match_offset = byteOffset;
    new_node->line_number = lineNumber;
    new_node->is_match = valid;
    new_node->aParameterMatch = aParameterMatch;
    new_node->reported = false;
    new_node->next = NULL;

    return new_node;
}
