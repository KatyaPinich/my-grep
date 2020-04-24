#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "regular_expression.h"
#include "command_line_parser.h"

Expression* CreateExpression(ExpressionElement **elements, int element_count);
ExpressionElement* CreateExpressionElement(RegexType element_type, char expression_char);
void FreeElements(ExpressionElement **elements, int element_count);
bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index);

Expression* ParseExpression(const char *expression_string)
{
    Expression *expression;
    ExpressionElement **elements;
    int expression_length;
    int i = 0;
    int element_count = 0;

    expression_length = strlen(expression_string);

    elements = calloc(expression_length, sizeof(*elements));
    if (elements == NULL)
        return NULL;

    while (expression_string[i] != '\0')
    {
        if (expression_string[i] == '\\')
        {
            i++;
        }

        // For now we only have regular characters
        elements[element_count] = CreateExpressionElement(REGEX_CHAR, expression_string[i]);
        if (elements[element_count] == NULL)
        {
            FreeElements(elements, element_count);
            exit(EXIT_FAILURE);
        }
        element_count++;

        i++;
    }

    expression = CreateExpression(elements, element_count);

    return expression;
}

ExpressionElement* CreateExpressionElement(RegexType element_type, char expression_char)
{
    ExpressionElement *new_element = (ExpressionElement*)malloc(sizeof(ExpressionElement));
    if (new_element != NULL)
    {
        new_element->type = element_type;
        new_element->value = expression_char;
    }

    return new_element;
}

Expression* CreateExpression(ExpressionElement **elements, int element_count)
{
    Expression *expression = (Expression*)malloc(sizeof(Expression));
    if (expression != NULL)
    {
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

    for (i = 0; i < element_count; ++i)
    {
        free(elements[i]);
    }
}

bool IsMatchInLine(const char *line, Expression *expression, bool exact_match)
{
    int i = 0;
    int expression_index = 0;

    while (line[i] != '\0')
    {
        if (IsMatchAtPlace(i, line, expression, expression_index))
        {
            return true;
        }

        i++;
    }

    return false;
}

// TODO: For now we handle the easiest case which is just matching the character
bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index)
{
    if (expression_index >= expression->element_count)
        return true;

    if (line[at_place] == '\0')
        return expression_index == expression->element_count;

    if (line[at_place] != expression->elements[expression_index]->value)
    {
        return false;
    }
    else
    {
        return IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1);
    }
}