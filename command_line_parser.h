#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <stdbool.h>

typedef enum InputMode
{
    INPUT_FILE,
    INPUT_STDIN
} InputMode;

typedef struct Parameters
{
    char *expression;
    InputMode input_mode;
    char *filename;
    int aParameter;
    bool bParameter;
    bool cParameter;
    bool ignore_case;
    bool nParameter;
    bool invert_match;
    bool xParameter;
    bool eParameter;
} Parameters;

Parameters* ParseParameters(int argc, char *argv[]);
void FreeParameters(Parameters *parameters);

#endif
