#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "regular_expression.h"
#include "command_line_parser.h"
#include "string_tools.h"

Expression* CreateExpression(ExpressionElement **elements, int element_count);
ExpressionElement* CreateExpressionElement(RegexType element_type, char expression_char, char expression_char2,
        bool emptyFirstTerm, bool emptySecondTerm);
void FreeElements(ExpressionElement **elements, int element_count);
bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index, bool exact_match,
                    bool prevFirstTermMatch, bool prevSecondTermMatch);
int FindOrTerm (char **str, int index, const char *expression_string, char stopChar);
int FindOrTerms (char **firstOrTerm, char **secondOrTerm, const char *expression_string, int index);
unsigned int MinNum (int a, int b);
bool IsRegexOrMatchAtPlace(const char *line, int at_place, Expression *expression,
                           int expression_index, bool exact_match, bool firstTermMatch, bool secondTermMatch);

Expression* ParseExpression(const char *expression_string)
{
    Expression *expression;
    ExpressionElement **elements;
    int expression_length;
    int i = 0;
    unsigned int j;
    int element_count = 0, firstOrTermLength, secondOrTermLength;
    bool backslash = false;
    char *firstOrTerm, *secondOrTerm, lowRangeChar, highRangeChar;

    expression_length = strlen(expression_string);

    elements = calloc(expression_length, sizeof(*elements));
    if (elements == NULL)
    {
        return NULL;
    }
    while (expression_string[i] != '\0')
    {
        if (expression_string[i] == '\\')
        {
            backslash = true;
            i++;
        }

        if (expression_string[i] == '(' && !backslash)
        {
            i = i + FindOrTerms(&firstOrTerm, &secondOrTerm, expression_string, i);
            if (firstOrTerm != NULL)
            {
                firstOrTermLength = strlen(firstOrTerm);
            }
            else
            {
                firstOrTermLength = 0;
            }
            if (secondOrTerm != NULL)
            {
                secondOrTermLength = strlen(secondOrTerm);
            }
            else
            {
                secondOrTermLength = 0;
            }
            for (j = 0; j < MinNum(firstOrTermLength, secondOrTermLength); j++)
            {
                elements[element_count] = CreateExpressionElement(REGEX_OR, firstOrTerm[j],
                        secondOrTerm[j], false, false);
                element_count++;
            }
            while (j < firstOrTermLength)
            {
                elements[element_count] = CreateExpressionElement(REGEX_OR, firstOrTerm[j],
                        ' ', false, true);
                element_count++;
                j++;
            }
            while (j < secondOrTermLength)
            {
                elements[element_count] = CreateExpressionElement(REGEX_OR, ' ',
                        secondOrTerm[j], true, false);
                element_count++;
                j++;
            }
            free(firstOrTerm);
            free(secondOrTerm);
            elements[element_count - 1]->lastOrType = true;
            continue;
        }
        else if (expression_string[i] == '[' && !backslash)
        {
            i++;
            lowRangeChar = expression_string[i];
            i = i + 2;
            highRangeChar = expression_string[i];
            elements[element_count] = CreateExpressionElement(REGEX_RANGE,
                    lowRangeChar, highRangeChar, false, false);
            i++;
        }
        else if (expression_string[i] == '.' && !backslash)
        {
            elements[element_count] = CreateExpressionElement(REGEX_WILDCARD, expression_string[i],
                    ' ', false, false);
        }
        else
        {
            elements[element_count] = CreateExpressionElement(REGEX_CHAR, expression_string[i],
                    ' ', false, false);
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

unsigned int MinNum (int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}

int FindOrTerms (char **firstOrTerm, char **secondOrTerm, const char *expression_string, int index)
{
    int firstTermLength;
    firstTermLength = FindOrTerm(firstOrTerm, index, expression_string, '|');
    index = index + firstTermLength;
    return 1 + firstTermLength + FindOrTerm(secondOrTerm, index, expression_string, ')');
}

int FindOrTerm (char **str, int index, const char *expression_string, char stopChar)
{
    int firstCharIndex, count = 0;
    index++;
    firstCharIndex = index;
    while (expression_string[index] != stopChar)
    {
        if (expression_string[index] == '\\')
        {
            index++;
            continue;
        }
        index++;
        count++;
    }
    if (count == 0)
    {
        *str = NULL;
    }
    else
    {
        *str = (char*)malloc((count + 1) * sizeof(char));
        RoundBracketTermCopy(str, &expression_string[firstCharIndex], count);
        (*str)[count] = '\0';
    }
    return count + 1;
}

ExpressionElement* CreateExpressionElement(RegexType element_type, char expression_char, char expression_char2,
        bool emptyFirstTerm, bool emptySecondTerm)
{
    ExpressionElement *new_element = (ExpressionElement*)malloc(sizeof(ExpressionElement));
    if (new_element != NULL)
    {
        new_element->type = element_type;
        new_element->value1 = expression_char;
        new_element->value2 = expression_char2;
        new_element->emptyFirstTerm = emptyFirstTerm;
        new_element->emptySecondTerm = emptySecondTerm;
        new_element->lastOrType = false;
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
        if (IsMatchAtPlace(i, line, expression, expression_index, exact_match, true, true))
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

bool IsMatchAtPlace(int at_place, const char *line, Expression *expression, int expression_index, bool exact_match,
        bool prevFirstTermMatch, bool prevSecondTermMatch)
{
    char elementValue, elementValue2;
    RegexType elementType;
    if (expression_index >= expression->element_count || expression->elements[expression_index] == NULL)
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
    {
        return expression_index == expression->element_count;
    }
    elementValue = expression->elements[expression_index]->value1;
    elementValue2 = expression->elements[expression_index]->value2;
    elementType = expression->elements[expression_index]->type;
    if (expression_index > 0)
    {
        if (expression->elements[expression_index - 1]->lastOrType)
        {
            prevFirstTermMatch = true;
            prevSecondTermMatch = true;
        }
    }
    if (elementType == REGEX_OR)
    {
        return IsRegexOrMatchAtPlace(line, at_place, expression, expression_index, exact_match,
                prevFirstTermMatch, prevSecondTermMatch);
    }
    if (line[at_place] != elementValue)
    {
        if (elementType == REGEX_RANGE && (line[at_place] < elementValue || line[at_place] > elementValue2))
        {
            return false;
        }
        else if (elementType != REGEX_WILDCARD && elementType != REGEX_RANGE)
        {
            return false;
        }
    }

    return IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1, exact_match,
            prevFirstTermMatch, prevSecondTermMatch);
}

bool IsRegexOrMatchAtPlace(const char *line, int at_place, Expression *expression,
        int expression_index, bool exact_match, bool prevFirstTermMatch, bool prevSecondTermMatch)
{
    char elementValue = expression->elements[expression_index]->value1;
    char elementValue2 = expression->elements[expression_index]->value2;
    int tempIndex;
    bool term1Match, term2Match, firstTermMatch = true, secondTermMatch = true;
    bool emptyFirstTerm = expression->elements[expression_index]->emptyFirstTerm;
    bool emptySecondTerm = expression->elements[expression_index]->emptySecondTerm;

    if (elementValue != line[at_place] && !emptyFirstTerm)
    {
        firstTermMatch = false;
        if (elementValue2 != line[at_place] && !emptySecondTerm)
        {
            return false;
        }
    }
    if (elementValue2 != line[at_place] && !emptySecondTerm)
    {
        secondTermMatch = false;
    }
    if (expression_index > 0)
    {
        if (!prevSecondTermMatch)
            secondTermMatch = false;
        if (!prevFirstTermMatch)
            firstTermMatch = false;
    }
    if (!secondTermMatch && !firstTermMatch)
        return false;
    if (emptySecondTerm && emptyFirstTerm && secondTermMatch && firstTermMatch)
    {
        return IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1, exact_match,
                firstTermMatch, secondTermMatch);
    }
    if (emptySecondTerm && secondTermMatch)
    {
        tempIndex = expression_index;
        while (expression->elements[tempIndex]->emptySecondTerm)
        {
            tempIndex++;
            if (expression->elements[tempIndex] == NULL)
            {
                break;
            }
        }
        if (firstTermMatch)
            term1Match = IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1,
                    exact_match, firstTermMatch, secondTermMatch);
        else
            term1Match = false;
        term2Match = IsMatchAtPlace(at_place, line, expression, tempIndex, exact_match, firstTermMatch, true);
        return term2Match || term1Match;
    }
    if (emptyFirstTerm && firstTermMatch)
    {
        tempIndex = expression_index;
        while (expression->elements[tempIndex]->emptyFirstTerm)
        {
            tempIndex++;
            if (expression->elements[tempIndex] == NULL)
            {
                break;
            }
        }
        term1Match = IsMatchAtPlace(at_place, line, expression, tempIndex, exact_match, true, secondTermMatch);
        if (secondTermMatch)
            term2Match = IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1,
                    exact_match, firstTermMatch, secondTermMatch);
        else
            term2Match = false;
        return term1Match || term2Match;
    }
    return IsMatchAtPlace(at_place + 1, line, expression, expression_index + 1, exact_match,
            firstTermMatch, secondTermMatch);
}