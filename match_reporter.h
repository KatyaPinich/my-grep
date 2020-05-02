#ifndef MY_GREP_MATCH_REPORTER_H
#define MY_GREP_MATCH_REPORTER_H

#include <stdbool.h>

#include "command_line_parser.h"
#include "linked_list.h"

bool ReportLine(Node *line, bool invert_match);
void PrintLinesAfterContextSeparator(int match_count, int lines_after_context, struct Node *previousLine);
void ReportLineMatch(struct Node *line, Parameters *parameters);

#endif  // MY_GREP_MATCH_REPORTER_H
