#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "regular_expression.h"
#include "command_line_parser.h"

Expression* CreateExpression(ExpressionElement **elements, int element_count);
ExpressionElement* CreateExpressionElement(RegexType element_type, char expression_char);
void FreeElements(ExpressionElement **elements, int element_count);
bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index, bool exact_match);

Expression* ParseExpression(const char *expression_string)
{
    Expression *expression;
    ExpressionElement **elements;
    int expression_length;
    int i = 0;
    int element_count = 0;
    bool backslash = false;

    expression_length = strlen(expression_string);

    elements = calloc(expression_length, sizeof(*elements));
    if (elements == NULL)
        return NULL;

    while (expression_string[i] != '\0')
    {
        if (expression_string[i] == '\\')
        {
            backslash = true;
            i++;
        }

        // For now we only have regular characters and '.'
        if (expression_string[i] == '.' && !backslash)
        {
            elements[element_count] = CreateExpressionElement(REGEX_WILDCARD, expression_string[i]);
        }
        else
        {
            elements[element_count] = CreateExpressionElement(REGEX_CHAR, expression_string[i]);
        }
        if (elements[element_count] == NULL)
        {
            FreeElements(elements, element_count);
            return NULL;
        }
        element_count++;
        backslash = false;
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
        if (IsMatchAtPlace(i, line, expression, expression_index, exact_match))
        {
            return true;
        }

        if (exact_match)
        {
            break;
        }

        i++;
    }

    return false;
}

// TODO: For now we handle just matching the character and '.'
bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index, bool exact_match)
{
    char elementValue;
    RegexType elementType;
    if (expression_index >= expression->element_count)
    {
        if (!exact_match || line[at_place] == '\n' || line[at_place] == '\0')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if (line[at_place] == '\0')
        return expression_index == expression->element_count;
    elementValue = expression->elements[expression_index]->value;
    elementType = expression->elements[expression_index]->type;
    if (line[at_place] != elementValue && elementType != REGEX_WILDCARD)
    {
        return false;
    }
    else
    {
        return IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1, exact_match);
    }
}
