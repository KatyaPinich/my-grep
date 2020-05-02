#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "regular_expression.h"
#include "string_tools.h"

#define RANGE_START_OFFSET 1
#define RANGE_END_OFFSET 3

Expression *CreateExpression(ExpressionElement **elements, int element_count);
ExpressionElement *CreateExpressionElement(RegexType element_type, ElementInfo *element_info);
void FreeElements(ExpressionElement **elements, int element_count);
bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index, bool exact_match);
bool IsRegexOrMatchAtPlace(const char *line, int at_place, Expression *expression, int expression_index,
                           bool exact_match);
ExpressionElement *CreateCharElement(char value);
ExpressionElement *CreateWildcardElement();
ExpressionElement *CreateRangeElement(const char *expression_string, int open_bracket_index);
ExpressionElement *CreateOrElement(const char *expression_string, int open_brace_index, int close_brace_index);
int IndexOfChar(const char *str, char value, int start_index);
char *CopySubstring(const char *source, int start_index, int count);

Expression *ParseExpression(const char *expression_string)
{
  Expression *expression;
  ExpressionElement **elements;
  int expression_length;
  int i = 0;
  int element_count = 0;
  int closing_index;

  expression_length = strlen(expression_string);

  elements = malloc(sizeof(*elements) * expression_length);
  if (elements == NULL) {
    return NULL;
  }

  while (expression_string[i] != '\0') {
    switch (expression_string[i]) {
      case '\\':
        i++;
        elements[element_count] = CreateCharElement(expression_string[i]);
        break;
      case '.':
        elements[element_count] = CreateWildcardElement();
        break;
      case '[':
        closing_index = IndexOfChar(expression_string, ']', i);
        elements[element_count] = CreateRangeElement(expression_string, i);
        i = closing_index;
        break;
      case '(':
        closing_index = IndexOfChar(expression_string, ')', i);
        elements[element_count] = CreateOrElement(expression_string, i, closing_index);
        i = closing_index;
        break;
      default:
        elements[element_count] = CreateCharElement(expression_string[i]);
        break;
    }

    if (elements[element_count] == NULL) {
      FreeElements(elements, element_count);
      return NULL;
    }

    element_count++;
    i++;
  }

  expression = CreateExpression(elements, element_count);

  return expression;
}

ExpressionElement *CreateCharElement(char value)
{
  ElementInfo *element_info;

  element_info = (ElementInfo *)malloc(sizeof(ElementInfo));
  if (element_info == NULL) {
    return NULL;
  }

  element_info->value = value;
  return CreateExpressionElement(REGEX_CHAR, element_info);
}

ExpressionElement *CreateWildcardElement()
{
  ElementInfo *element_info;

  element_info = (ElementInfo *)malloc(sizeof(ElementInfo));
  if (element_info == NULL) {
    return NULL;
  }

  element_info->value = '.';
  return CreateExpressionElement(REGEX_WILDCARD, element_info);
}

ExpressionElement *CreateRangeElement(const char *expression_string, int open_bracket_index)
{
  ElementInfo *element_info;
  char range_start;
  char range_end;

  element_info = (ElementInfo *)malloc(sizeof(ElementInfo));
  if (element_info == NULL) {
    return NULL;
  }

  element_info->range = (RangeElement *)malloc(sizeof(RangeElement));
  if (element_info->range == NULL) {
    return NULL;
  }

  range_start = expression_string[open_bracket_index + RANGE_START_OFFSET];
  range_end = expression_string[open_bracket_index + RANGE_END_OFFSET];

  element_info->range->start = range_start;
  element_info->range->end = range_end;

  return CreateExpressionElement(REGEX_RANGE, element_info);
}

ExpressionElement *CreateOrElement(const char *expression_string, int open_brace_index, int close_brace_index)
{
  ElementInfo *element_info;
  int or_index;
  int first_option_length;
  int second_option_length;
  char *first_option;
  char *second_option;

  or_index = IndexOfChar(expression_string, '|', open_brace_index);
  first_option_length = or_index - open_brace_index - 1;
  second_option_length = close_brace_index - or_index - 1;

  element_info = (ElementInfo *)calloc(1, sizeof(ElementInfo));
  if (element_info == NULL) {
    return NULL;
  }

  element_info->alternation = (OrElement *)calloc(1, sizeof(OrElement));
  if (element_info->alternation == NULL) {
    return NULL;
  }

  if (first_option_length > 0 && second_option_length > 0) {
    first_option = CopySubstring(expression_string, open_brace_index + 1, first_option_length);
    second_option = CopySubstring(expression_string, or_index + 1, second_option_length);
    if (first_option == NULL || second_option == NULL) {
      return NULL;
    }

    element_info->alternation->first_option = first_option;
    element_info->alternation->second_option = second_option;
    element_info->alternation->optional = false;

  } else if (first_option_length > 0) {
    first_option = CopySubstring(expression_string, open_brace_index + 1, first_option_length);
    if (first_option == NULL) {
      return NULL;
    }
    element_info->alternation->first_option = first_option;
    element_info->alternation->second_option = NULL;
    element_info->alternation->optional = true;
  } else {
    second_option = CopySubstring(expression_string, or_index + 1, second_option_length);
    if (second_option == NULL) {
      return NULL;
    }
    element_info->alternation->first_option = second_option;
    element_info->alternation->second_option = NULL;
    element_info->alternation->optional = true;
  }

  return CreateExpressionElement(REGEX_OR, element_info);
}

char *CopySubstring(const char *source, int start_index, int count)
{
  int destination_length;
  char *destination;

  destination_length = count + 1;
  destination = (char *)calloc(destination_length, sizeof(char));
  if (destination != NULL) {
    strncpy(destination, &(source[start_index]), count);
  }

  return destination;
}

int IndexOfChar(const char *str, char value, int start_index)
{
  int char_index = -1;
  const char *result;

  result = strchr(&(str[start_index]), value);
  if (result != NULL) {
    char_index = (int)(result - str);
  }

  return char_index;
}

ExpressionElement *CreateExpressionElement(RegexType element_type, ElementInfo *element_info)
{
  ExpressionElement *new_element = (ExpressionElement *)malloc(sizeof(ExpressionElement));
  if (new_element != NULL) {
    new_element->type = element_type;
    new_element->info = element_info;
  }

  return new_element;
}

Expression *CreateExpression(ExpressionElement **elements, int element_count)
{
  Expression *expression = (Expression *)malloc(sizeof(Expression));
  if (expression != NULL) {
    expression->elements = elements;
    expression->element_count = element_count;
  }

  return expression;
}

void FreeExpression(Expression *expression)
{
  FreeElements(expression->elements, expression->element_count);

  free(expression->elements);
  free(expression);
}

void FreeElements(ExpressionElement **elements, int element_count)
{
  int i;

  for (i = 0; i < element_count; ++i) {
    if (elements[i]->type == REGEX_RANGE) {
      free(elements[i]->info->range);
    } else if (elements[i]->type == REGEX_OR) {
      free(elements[i]->info->alternation->first_option);
      free(elements[i]->info->alternation->second_option);
      free(elements[i]->info->alternation);
    }
    free(elements[i]->info);
    free(elements[i]);
  }
}

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