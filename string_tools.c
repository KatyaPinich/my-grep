#include "string_tools.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char* CopyString(const char *source_string)
{
    char *destination_string;
    int source_string_length;

    source_string_length = strlen(source_string) + 1;
    destination_string = (char*)malloc(sizeof(char) * source_string_length);
    if (destination_string == NULL)
        return NULL;

    strncpy(destination_string, source_string, source_string_length);
    return destination_string;
}

char* ToLowercaseString(const char *source_string)
{
    char *destination_string;
    int source_string_length, i;

    source_string_length = strlen(source_string) + 1;
    destination_string = (char*)malloc(sizeof(char) * source_string_length);
    if (destination_string == NULL)
        return NULL;

    for(i = 0; i < source_string_length; i++)
    {
        destination_string[i] = tolower(source_string[i]);
    }

    return destination_string;
}

