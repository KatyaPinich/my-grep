#include "StringTools.h"

#include <stdlib.h>
#include <string.h>


char* CopyString(const char *sourceString)
{
    char *destinationString;
    int sourceStringLength;

    sourceStringLength = strlen(sourceString) + 1;
    destinationString = (char*)malloc(sizeof(char) * sourceStringLength);
    if (destinationString == NULL)
        return NULL;

    strncpy(destinationString, sourceString, sourceStringLength);
    return destinationString;
}