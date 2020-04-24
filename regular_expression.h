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
} ExpressionElement;

ExpressionElement** ParseExpression(const char *expression_string);

#endif
