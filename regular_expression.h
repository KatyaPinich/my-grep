#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

typedef enum RegexType
{
    REGEX_WILDCARD,
    REGEX_OR,
    REGEX_RANGE,
    REGEX_CHAR
} RegexType;

typedef struct ExpressionElement
{
    RegexType type;
    char value;
    char rangeValue;
} ExpressionElement;

typedef struct Expression
{
    struct ExpressionElement **elements;
    int element_count;
} Expression;

Expression* ParseExpression(const char *expression_string);
bool IsMatchInLine(const char *line, Expression *expression, bool exact_match);
void FreeExpression(Expression *expression);

#endif
