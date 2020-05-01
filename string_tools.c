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

void RoundBracketTermCopy(char **destinationString, const char *sourceString, int count)
{
    int i, j = 0;
    for (i = 0; i < count; i++)
    {
        if (sourceString[j] == '\\')
        {
            i--;
            j++;
            continue;
        }
        (*destinationString)[i] = sourceString[j];
        j++;
    }
}

char* ToLowercaseString(const char *source_string)
{
    char *destination_string;
    int source_string_length;
    int i;

    source_string_length = strlen(source_string) + 1;
    destination_string = (char*)malloc(sizeof(char) * source_string_length);
    if (destination_string == NULL)
        return NULL;

    for (i = 0; i < source_string_length; ++i)
    {
        destination_string[i] = tolower(source_string[i]);
    }

    return destination_string;
}

