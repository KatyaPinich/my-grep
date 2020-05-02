#ifndef MY_GREP_MATCH_FINDER_H
#define MY_GREP_MATCH_FINDER_H

#include "regular_expression.h"

bool IsMatchInLine(const char *line, Expression *expression, bool exact_match);

#endif  // MY_GREP_MATCH_FINDER_H
