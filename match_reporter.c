#include <stdio.h>
#include <stdlib.h>

#include "match_reporter.h"

void ReportLineAfterContext(Node *line_after_context, Parameters *parameters);
void PrintLineMatch(struct Node *line, Parameters *parameters, char separator);

void PrintLinesAfterContextSeparator(int match_count, int lines_after_context, struct Node *previousLine)
{
  if (match_count > 0 && lines_after_context > 0) {
    if (!previousLine->reported) {
      printf("--\n");
    }
  }
}

void ReportLineMatch(struct Node *line, Parameters *parameters)
{
  int reported_after_context;
  Node *line_after_context;

  if (!line->reported) {
    PrintLineMatch(line, parameters, ':');
    line->reported = true;
  }

  if (parameters->lines_after_context > 0) {
    line_after_context = line->next;
    if (line_after_context == NULL) {
      return;
    }
    reported_after_context = parameters->lines_after_context;
    while (reported_after_context > 0 && line_after_context->next != NULL) {
      if (!line_after_context->reported) {
        ReportLineAfterContext(line_after_context, parameters);
      }

      line_after_context = line_after_context->next;
      reported_after_context--;
    }
  }
}

bool ReportLine(Node *line, bool invert_match)
{
  if (!invert_match) {
    return line->is_match;
  } else {
    return !line->is_match;
  }
}

void PrintLineMatch(struct Node *line, Parameters *parameters, char separator)
{
  if (parameters->print_line_count)  // print only line numbers
  {
    printf("%d\n", line->line_number);
  } else if (parameters->line_number)  // print line number before every line
  {
    if (parameters->byte_offset)  // print byte offset before line
    {
      printf("%d%c%d%c%s", line->line_number, separator, line->match_offset, separator, line->line);
    } else {
      printf("%d%c%s", line->line_number, separator, line->line);
    }
  } else if (parameters->byte_offset)  // print byte offset before line
  {
    printf("%d%c%s", line->match_offset, separator, line->line);
  } else {
    printf("%s", line->line);
  }
}

void ReportLineAfterContext(Node *line_after_context, Parameters *parameters)
{
  if (ReportLine(line_after_context, parameters->invert_match)) {
    PrintLineMatch(line_after_context, parameters, ':');
  } else {
    PrintLineMatch(line_after_context, parameters, '-');
  }

  line_after_context->reported = true;
}