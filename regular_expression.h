#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

typedef enum RegexType { REGEX_WILDCARD, REGEX_OR, REGEX_RANGE, REGEX_CHAR } RegexType;

typedef struct RangeExpression {
    char start;
    char end;
} RangeExpression;

typedef struct OrExpression {
    char *first_option;
    char *second_option;
    bool optional;
} OrExpression;

typedef union ElementInfo {
    char value;
    RangeExpression *range;
    OrExpression *alternation;
} ElementInfo;

typedef struct ExpressionElement {
  RegexType type;
  char value1;
  char value2;
  bool emptyFirstTerm;
  bool emptySecondTerm;
  bool lastOrType;
  ElementInfo* info;
} ExpressionElement;

typedef struct Expression {
  struct ExpressionElement **elements;
  int element_count;
} Expression;

Expression *ParseExpression(const char *expression_string);
bool IsMatchInLine(const char *line, Expression *expression, bool exact_match);
void FreeExpression(Expression *expression);

#endif
