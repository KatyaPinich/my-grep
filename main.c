#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "command_line_parser.h"

void Grep(Parameters *parameters);
char* ReadLineFromFile(const char *filename);
char* ReadLine(Parameters *parameters);
bool IsMatchInLine(const char *expression, const char *line);

int main(int argc, char *argv[])
{
    Parameters *parameters;

    parameters = ParseParameters(argc, argv);
    Grep(parameters);

    return 0;
}

void Grep(Parameters *parameters)
{
    char *line;

    line = ReadLine(parameters);
    printf("Read Line: %s\n", line);

    if (IsMatchInLine(parameters->expression, line))
    {
        printf("%s\n", line);
    }
}

char* ReadLine(Parameters *parameters)
{
    if (parameters->inputMode == INPUT_FILE)
    {
        return ReadLineFromFile(parameters->filename);
    }

    return NULL;
}

char* ReadLineFromFile(const char *filename)
{
    FILE *file;
    char *line;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s for reading.\n", filename);
        return NULL;
    }

    fscanf(file, "%s\n", line);

    fclose(file);

    return line;
}

bool IsMatchInLine(const char *expression, const char *line)
{
    return (strncmp(expression, line, strlen(expression)) == 0);
}



