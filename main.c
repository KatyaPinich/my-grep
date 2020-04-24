#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "command_line_parser.h"
#include "linked_list.h"
#include "string_tools.h"

void Grep(Parameters *parameters);
char* ReadLine(FILE* input_stream);
bool IsMatchInLine(Parameters *parameters, const char *line);
FILE* GetInputStream(Parameters *parameters);
FILE* OpenFile(const char *filename);
void ReportLineMatch(struct Node* line, Parameters *parameters);
void FillLinesStruct(Parameters *parameters, struct Node* *lines, FILE* input_stream);
void PrintLineMatch(struct Node* line, Parameters *parameters, char separator);

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
    struct Node *lines = NULL, *line = NULL;

    input_stream = GetInputStream(parameters);

    FillLinesStruct(parameters, &lines, input_stream);

    line = lines;
    if (line == NULL)
    {
        ReportLineMatch(line, parameters);
    }
    while (line != NULL)
    {
        ReportLineMatch(line, parameters);
        line = line->next;
    }

    if (parameters->input_mode == INPUT_FILE)
        fclose(input_stream);

    FreeLinkedList(&lines);
}

void FillLinesStruct(Parameters *parameters, struct Node* *lines, FILE* input_stream)
{
    int bytes_read = 0, lineNumber = 1, aParameter = 0;
    bool has_match, aParameterMatch;
    char *line;
    char *lineToMatch;

    if (parameters->ignore_case)
    {
        parameters->expression = ToLowercaseString(parameters->expression);
        if (parameters->expression == NULL)
        {
            FreeParameters(parameters);
            exit(EXIT_FAILURE);
        }
    }

    line = ReadLine(input_stream);
    while (line != NULL)
    {
        if (parameters->ignore_case)
        {
            lineToMatch = ToLowercaseString(line);
            if (lineToMatch == NULL)
            {
                FreeParameters(parameters);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            lineToMatch = line;
        }

        has_match = IsMatchInLine(parameters, lineToMatch);

        if (parameters->invert_match)
            has_match = !has_match;

        if (has_match && parameters->aParameter != -1)
        {
            aParameter = parameters->aParameter;
        }
        if (aParameter > 0)
        {
            aParameterMatch = true;
            aParameter--;
        }
        else
        {
            aParameterMatch = false;
        }
        AddToEndOfLinkedList(lines, line, has_match, bytes_read, lineNumber, aParameterMatch);
        bytes_read += strlen(line);
        line = ReadLine(input_stream);
        lineNumber++;
    }
}

FILE* GetInputStream(Parameters *parameters)
{
    if (parameters->input_mode == INPUT_FILE)
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

    if (parameters->xParameter) //find exact lines only
    {
        match = strcmp(parameters->expression, line) == 0;
    }
    else
    {
        match = strstr(line, parameters->expression) != NULL;
    }
    return match;
}

void ReportLineMatch(struct Node* line, Parameters *parameters)
{
    if (line == NULL)
    {
        return;
    }
    else if (line->valid)
    {
        PrintLineMatch(line, parameters, ':');
    }
    else if (line->aParameterMatch)
    {
        PrintLineMatch(line, parameters, '-');
    }
}

void PrintLineMatch(struct Node* line, Parameters *parameters, char separator)
{
    if (parameters->cParameter) //print only line numbers
    {
        printf("%d\n", line->lineNumber);
    } else if (parameters->nParameter) //print line number before every line
    {
        if (parameters->bParameter) //print byte offset before line
        {
            printf("%d%c%d%c%s", line->lineNumber, separator, line->byteOffset, separator, line->line);
        } else {
            printf("%d%c%s", line->lineNumber, separator, line->line);
        }
    } else if (parameters->bParameter) //print byte offset before line
    {
        printf("%d%c%s", line->byteOffset, separator, line->line);
    } else {
        printf("%s", line->line);
    }
}