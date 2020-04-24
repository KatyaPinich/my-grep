#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "regular_expression.h"

ExpressionElement* CreateExpressionElement(RegexType element_type, char expression_char);
void FreeExpression(ExpressionElement *expression[], int element_count);

ExpressionElement** ParseExpression(const char *expression_string)
{
    ExpressionElement **expression;
    int expression_length;
    int i = 0;
    int element_count = 0;

    expression_length = strlen(expression_string);

    expression = calloc(expression_length, sizeof(*expression));
    if (expression == NULL)
        return NULL;

    while (expression_string[i] != '\0')
    {
        if (expression_string[i] == '\\')
        {
            i++;
        }

        // For now we only have regular characters
        expression[element_count] = CreateExpressionElement(REGEX_CHAR, expression_string[i]);
        if (expression[element_count] == NULL)
        {
            FreeExpression(expression, element_count);
            exit(EXIT_FAILURE);
        }
        element_count++;

        i++;
    }

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

void FreeExpression(ExpressionElement *expression[], int element_count)
{
    int i;

    for (i = 0; i < element_count; ++i)
    {
        free(expression[i]);
    }

    free(expression);
}
