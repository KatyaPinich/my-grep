#ifndef MY_GREP_MATCH_FINDER_H
#define MY_GREP_MATCH_FINDER_H

#include <stdio.h>

#include "regular_expression.h"
#include "command_line_parser.h"
#include "linked_list.h"

void MatchLines(Parameters *parameters, struct Node **lines, FILE *input_stream);

#endif  // MY_GREP_MATCH_FINDER_H
