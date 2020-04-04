#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "command_line_parser.h"

void Grep(Parameters *parameters);
char* ReadLineFromFile(const char *filename, int start_from);
char* ReadLine(Parameters *parameters, int read_from);
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
    int bytes_read = 0;

    line = ReadLine(parameters, bytes_read);
    while (line != NULL)
    {
        //printf("Read line: %s", line);
        if (IsMatchInLine(parameters->expression, line))
        {
            printf("%s", line);
        }

        bytes_read += strlen(line);
        line = ReadLine(parameters, bytes_read);
    }
}

char* ReadLine(Parameters *parameters, int read_from)
{
    if (parameters->inputMode == INPUT_FILE)
    {
        return ReadLineFromFile(parameters->filename, read_from);
    }

    return NULL;
}

char* ReadLineFromFile(const char *filename, int start_from)
{
    FILE *file;
    char *line = NULL;
    size_t line_length = 0;
    ssize_t bytes_read;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s for reading.\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, start_from, SEEK_SET);
    bytes_read = getline(&line, &line_length, file);
    if (bytes_read == -1)
    {
        line = NULL;
    }

    fclose(file);

    return line;
}

bool IsMatchInLine(const char *expression, const char *line)
{
    return (strncmp(expression, line, strlen(expression)) == 0);
}



