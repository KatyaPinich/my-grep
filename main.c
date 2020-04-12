#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "command_line_parser.h"
#include "string_tools.h"

void Grep(Parameters *parameters);
char* ReadLine(FILE* input_stream);
bool IsMatchInLine(Parameters *parameters, const char *line);
FILE* GetInputStream(Parameters *parameters);
FILE* OpenFile(const char *filename);
void ReportLineMatch(int lineNumber, char *line, Parameters *parameters, int bytesRead);

int main(int argc, char *argv[])
{
    Parameters *parameters;

    parameters = ParseParameters(argc, argv);
    Grep(parameters);
    FreeParameters(parameters);

    return 0;
}

void Grep(Parameters *parameters)
{
    FILE* input_stream;
    char *line;
    int bytes_read = 0, lineNumber = 1;

    input_stream = GetInputStream(parameters);

    line = ReadLine(input_stream);
    while (line != NULL)
    {
        if (IsMatchInLine(parameters, line))
        {
            ReportLineMatch(lineNumber, line, parameters, bytes_read);
        }

        bytes_read += strlen(line);
        free(line);
        line = ReadLine(input_stream);
        lineNumber++;
    }

    if (parameters->inputMode == INPUT_FILE)
        fclose(input_stream);

    free(line);
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
        free(line);
        line = NULL;
    }

    return line;
}

bool IsMatchInLine(Parameters *parameters, const char *line)
{
    bool match;
    char *lowercaseExpression = NULL, *lowercaseLine = NULL;

    if(parameters->iParameter)
    {
        lowercaseExpression = ToLowercaseString(parameters->expression);
        lowercaseLine = ToLowercaseString(line);
        match = strncmp(lowercaseExpression, lowercaseLine, strlen(parameters->expression)) == 0;
        free(lowercaseExpression);
        free(lowercaseLine);
    }
    else
    {
        match = strncmp(parameters->expression, line, strlen(parameters->expression)) == 0;
    }

    if(parameters->vParameter)
    {
        match = !match;
    }
    return match;
}

void ReportLineMatch(int lineNumber, char *line, Parameters *parameters, int bytesRead)
{
    if (parameters->vParameter)
    {
        printf("%d\n", lineNumber);
    }
    else if (parameters->nParameter)
    {
        if(parameters->bParameter)
        {
            printf("%d:%d:%s", lineNumber, bytesRead, line);
        }
        else
        {
            printf("%d:%s", lineNumber, line);
        }
    }
    else if (parameters->bParameter)
    {
        printf("%d:%s", bytesRead, line);
    }
    else
    {
        printf("%s", line);
    }
}

