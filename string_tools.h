#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

char *CopyString(const char *source_string);
char *ToLowercaseString(const char *source_string);
char *CopySubstring(const char *source, int start_index, int count);
int IndexOfChar(const char *str, char value, int start_index);

#endif
