#include <stdbool.h>
#include <string.h>

#include "match_finder.h"

bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index, bool exact_match);
bool IsRegexOrMatchAtPlace(const char *line, int at_place, Expression *expression, int expression_index,
                           bool exact_match);

bool IsMatchInLine(const char *line, Expression *expression, bool exact_match)
{
  int i = 0;
  int expression_index = 0;

  while (line[i] != '\0') {
    if (IsMatchAtPlace(i, line, expression, expression_index, exact_match)) {
      return true;
    }

    if (exact_match) {
      break;
    }

    i++;
  }

  return false;
}

bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index, bool exact_match)
{
  RegexType element_type;
  ElementInfo *element_info;

  // We reached the end of the expression
  if (expression_index >= expression->element_count) {
    if (exact_match) {
      // We have an exact match only if we reached the end of the line
      return (line[at_place] == '\n' || line[at_place] == '\0');
    } else {
      // We reached the end of the expression and we are not looking for an exact match
      // return true
      return true;
    }
  }

  // If we reached the end of the line, but not the end of the expression return false
  if (line[at_place] == '\n' || line[at_place] == '\0') {
    return false;  // expression_index == expression->element_count;
  }

  element_type = expression->elements[expression_index]->type;
  element_info = expression->elements[expression_index]->info;

  switch (element_type) {
    case REGEX_CHAR:
      if (line[at_place] != element_info->value) {
        return false;
      }
      break;
    case REGEX_RANGE:
      if (line[at_place] < element_info->range->start || line[at_place] > element_info->range->end) {
        return false;
      }
      break;
    case REGEX_OR:
      return IsRegexOrMatchAtPlace(line, at_place, expression, expression_index, exact_match);
    case REGEX_WILDCARD:
    default:
      break;
  }

  return IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1, exact_match);
}

bool IsRegexOrMatchAtPlace(const char *line, int at_place, Expression *expression, int expression_index,
                           bool exact_match)
{
  OrElement *alternation;
  bool first_match = false;
  bool second_match = false;
  int first_option_length = 0, second_option_length = 0;

  alternation = expression->elements[expression_index]->info->alternation;
  if (alternation->first_option != NULL) {
    first_option_length = strlen(alternation->first_option);
  }
  if (alternation->second_option != NULL) {
    second_option_length = strlen(alternation->second_option);
  }
  if (strncmp(&(line[at_place]), alternation->first_option, first_option_length) == 0) {
    first_match = IsMatchAtPlace(at_place + first_option_length, line, expression, expression_index + 1, exact_match);
  }

  if (!alternation->optional) {
    if (strncmp(&(line[at_place]), alternation->second_option, second_option_length) == 0) {
      second_match =
          IsMatchAtPlace(at_place + second_option_length, line, expression, expression_index + 1, exact_match);
    }
  } else {
    second_match = IsMatchAtPlace(at_place, line, expression, expression_index + 1, exact_match);
  }

  return first_match || second_match;
}