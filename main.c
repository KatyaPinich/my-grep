#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "command_line_parser.h"
#include "linked_list.h"
#include "match_finder.h"
#include "match_reporter.h"
#include "regular_expression.h"
#include "stream_handler.h"
#include "string_tools.h"

void Grep(Parameters *parameters);
void FillLinesStruct(Parameters *parameters, struct Node **lines, FILE *input_stream);

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
  FILE *input_stream;
  struct Node *lines = NULL, *line = NULL, *previousLine = NULL;
  int match_count = 0;
  input_stream = GetInputStream(parameters);
  if (input_stream == NULL) {
    FreeParameters(parameters);
    exit(EXIT_FAILURE);
  }
  FillLinesStruct(parameters, &lines, input_stream);
  line = lines;
  while (line != NULL) {
    if (ReportLine(line, parameters->invert_match)) {
      if (!parameters->print_line_count) {
        PrintLinesAfterContextSeparator(match_count, parameters->lines_after_context, previousLine);
        ReportLineMatch(line, parameters);
      }
      match_count++;
    }
    previousLine = line;
    line = line->next;
  }
  if (parameters->print_line_count) {
    printf("%d\n", match_count);
  }
  if (parameters->input_mode == INPUT_FILE) {
    fclose(input_stream);
  }
  FreeLinkedList(&lines);
}

void FillLinesStruct(Parameters *parameters, struct Node **lines, FILE *input_stream)
{
  int bytes_read = 0, line_number = 1;
  bool has_match, aParameterMatch = false;
  char *line, *lineToMatch, *tempExpression;
  Expression *expression;

  if (parameters->ignore_case) {
    tempExpression = parameters->expression;
    parameters->expression = ToLowercaseString(parameters->expression);
    free(tempExpression);
    if (parameters->expression == NULL) {
      FreeParameters(parameters);
      exit(EXIT_FAILURE);
    }
  }

  expression = ParseExpression(parameters->expression);
  if (expression == NULL) {
    FreeParameters(parameters);
    exit(EXIT_FAILURE);
  }

  line = ReadLine(input_stream);
  while (line != NULL) {
    if (parameters->ignore_case) {
      lineToMatch = ToLowercaseString(line);
      if (lineToMatch == NULL) {
        FreeParameters(parameters);
        exit(EXIT_FAILURE);
      }
    } else {
      lineToMatch = line;
    }

    has_match = IsMatchInLine(lineToMatch, expression, parameters->exact_match);
    if (parameters->ignore_case) {
      free(lineToMatch);
    }
    if (AddToEndOfLinkedList(lines, line, has_match, bytes_read, line_number, aParameterMatch) == 1) {
      FreeParameters(parameters);
      exit(EXIT_FAILURE);
    }
    bytes_read += strlen(line);
    line = ReadLine(input_stream);
    line_number++;
  }
  FreeExpression(expression);
}