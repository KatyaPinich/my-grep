#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "string_tools.h"

char *CopyString(const char *source_string)
{
  char *destination_string;
  unsigned int source_string_length;

  source_string_length = strlen(source_string) + 1;
  destination_string = (char *)malloc(sizeof(char) * source_string_length);
  if (destination_string == NULL) {
    return NULL;
  }
  strncpy(destination_string, source_string, source_string_length);
  return destination_string;
}

char *ToLowercaseString(const char *source_string)
{
  char *destination_string;
  unsigned int source_string_length;
  int i;

  source_string_length = strlen(source_string) + 1;
  destination_string = (char *)malloc(sizeof(char) * source_string_length);
  if (destination_string == NULL) {
    return NULL;
  }
  for (i = 0; i < source_string_length; ++i) {
    destination_string[i] = tolower(source_string[i]);
  }

  return destination_string;
}

char *CopySubstring(const char *source, int start_index, int count)
{
  int destination_length;
  char *destination;

  destination_length = count + 1;
  destination = (char *)calloc(destination_length, sizeof(char));
  if (destination != NULL) {
    strncpy(destination, &(source[start_index]), count);
  }

  return destination;
}

int IndexOfChar(const char *str, char value, int start_index)
{
  int char_index = -1;
  const char *result;

  result = strchr(&(str[start_index]), value);
  if (result != NULL) {
    char_index = (int)(result - str);
  }

  return char_index;
}