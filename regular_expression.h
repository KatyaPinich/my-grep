#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

#include <stdbool.h>

typedef enum RegexType { REGEX_WILDCARD, REGEX_OR, REGEX_RANGE, REGEX_CHAR } RegexType;

typedef struct RangeElement {
  char start;
  char end;
} RangeElement;

typedef struct OrElement {
  char *first_option;
  char *second_option;
  bool optional;
} OrElement;

typedef union ElementInfo {
  char value;
  RangeElement *range;
  OrElement *alternation;
} ElementInfo;

typedef struct ExpressionElement {
  RegexType type;
  ElementInfo *info;
} ExpressionElement;

typedef struct Expression {
  struct ExpressionElement **elements;
  int element_count;
} Expression;

Expression *ParseExpression(const char *expression_string);
void FreeExpression(Expression *expression);

#endif
