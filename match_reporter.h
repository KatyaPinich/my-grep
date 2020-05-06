#ifndef MY_GREP_MATCH_REPORTER_H
#define MY_GREP_MATCH_REPORTER_H

#include <stdbool.h>

#include "command_line_parser.h"
#include "linked_list.h"

void ReportLines(Node *lines_list, Parameters *parameters);

#endif  // MY_GREP_MATCH_REPORTER_H
