#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "command_line_parser.h"

void Grep(Parameters *parameters);
char* ReadLine(FILE* input_stream);
bool IsMatchInLine(const char *expression, const char *line);
FILE* GetInputStream(Parameters *parameters);
FILE* OpenFile(const char *filename);

int main(int argc, char *argv[])
{
    Parameters *parameters;

    parameters = ParseParameters(argc, argv);
    Grep(parameters);

    return 0;
}

void Grep(Parameters *parameters)
{
    FILE* input_stream;
    char *line;
    int bytes_read = 0;

    input_stream = GetInputStream(parameters);

    line = ReadLine(input_stream);
    while (line != NULL)
    {
        if (IsMatchInLine(parameters->expression, line))
        {
            printf("%s", line);
        }

        bytes_read += strlen(line);
        line = ReadLine(input_stream);
    }

    if (parameters->inputMode == INPUT_FILE)
        fclose(input_stream);
}

FILE* GetInputStream(Parameters *parameters)
{
    if (parameters->inputMode == INPUT_FILE)
    {
        return OpenFile(parameters->filename);
    }
    else
    {
        return stdin;
    }
}

FILE* OpenFile(const char *filename)
{
    FILE *file;

    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s for reading.\n", filename);
        exit(EXIT_FAILURE);
    }

    return file;
}

char* ReadLine(FILE* input_stream)
{
    char *line = NULL;
    size_t line_length = 0;
    ssize_t bytes_read;

    bytes_read = getline(&line, &line_length, input_stream);
    if (bytes_read == -1)
    {
        line = NULL;
    }

    return line;
}

bool IsMatchInLine(const char *expression, const char *line)
{
    return (strncmp(expression, line, strlen(expression)) == 0);
}



