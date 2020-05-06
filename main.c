#include <stdlib.h>
#include <string.h>

#include "command_line_parser.h"
#include "linked_list.h"
#include "match_finder.h"
#include "match_reporter.h"
#include "stream_handler.h"

void Grep(Parameters *parameters);

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
  struct Node *lines_list = NULL, *line = NULL, *previousLine = NULL;
  int match_count = 0;
  input_stream = GetInputStream(parameters);
  if (input_stream == NULL) {
    FreeParameters(parameters);
    exit(EXIT_FAILURE);
  }
    MatchLines(parameters, &lines_list, input_stream);
  line = lines_list;
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
  FreeLinkedList(&lines_list);
}