#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "command_line_parser.h"
#include "linked_list.h"
#include "string_tools.h"
#include "stream_handler.h"
#include "regular_expression.h"

void Grep(Parameters *parameters);
void ReportLineMatch(struct Node* line, Parameters *parameters);
void FillLinesStruct(Parameters *parameters, struct Node* *lines, FILE* input_stream);
void PrintLineMatch(struct Node* line, Parameters *parameters, char separator);
bool ReportLine(Node *line, bool invert_match);

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
    int match_count = 0;

    input_stream = GetInputStream(parameters);

    FillLinesStruct(parameters, &lines, input_stream);

    line = lines;
    while (line != NULL)
    {
        if (ReportLine(line, parameters->invert_match))
        {
            if (parameters->print_line_count)
            {
                match_count++;
            }
            else
            {
                ReportLineMatch(line, parameters);
            }
        }

        line = line->next;
    }

    if (parameters->print_line_count)
    {
        printf("%d\n", match_count);
    }

    if (parameters->input_mode == INPUT_FILE)
        fclose(input_stream);

    FreeLinkedList(&lines);
}

void FillLinesStruct(Parameters *parameters, struct Node* *lines, FILE* input_stream)
{
    int bytes_read = 0, line_number = 1;
    bool has_match, aParameterMatch = false;
    char *line;
    char *lineToMatch;
    Expression *expression;

    if (parameters->ignore_case)
    {
        parameters->expression = ToLowercaseString(parameters->expression);
        if (parameters->expression == NULL)
        {
            FreeParameters(parameters);
            exit(EXIT_FAILURE);
        }
    }

    expression = ParseExpression(parameters->expression);

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

        has_match = IsMatchInLine(lineToMatch, expression, parameters->exact_match);

        AddToEndOfLinkedList(lines, line, has_match, bytes_read, line_number, aParameterMatch);
        bytes_read += strlen(line);
        line = ReadLine(input_stream);
        line_number++;
    }
    FreeExpression(expression);
}

void ReportLineMatch(struct Node* line, Parameters *parameters)
{
    int reported_after_context;
    Node *line_after_context;

    if (!line->reported)
    {
        PrintLineMatch(line, parameters, ':');
        line->reported = true;
    }

    if (parameters->lines_after_context > 0)
    {
        line_after_context = line->next;
        reported_after_context = parameters->lines_after_context;
        while (reported_after_context > 0 && line_after_context->next != NULL)
        {
            if (!line_after_context->reported)
            {
                if (ReportLine(line_after_context, parameters->invert_match))
                {
                    PrintLineMatch(line_after_context, parameters, ':');
                }
                else
                {
                    PrintLineMatch(line_after_context, parameters, '-');
                }

                line_after_context->reported = true;
            }

            line_after_context = line_after_context->next;
            reported_after_context--;
        }
    }
}

bool ReportLine(Node *line, bool invert_match)
{
    if (!invert_match)
    {
        return line->is_match;
    }
    else
    {
        return !line->is_match;
    }

}

void PrintLineMatch(struct Node* line, Parameters *parameters, char separator)
{
    if (parameters->print_line_count) //print only line numbers
    {
        printf("%d\n", line->line_number);
    } else if (parameters->line_number) //print line number before every line
    {
        if (parameters->byte_offset) //print byte offset before line
        {
            printf("%d%c%d%c%s", line->line_number, separator, line->match_offset, separator, line->line);
        } else {
            printf("%d%c%s", line->line_number, separator, line->line);
        }
    } else if (parameters->byte_offset) //print byte offset before line
    {
        printf("%d%c%s", line->match_offset, separator, line->line);
    } else {
        printf("%s", line->line);
    }
}