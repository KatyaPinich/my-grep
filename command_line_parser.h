#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

#include <stdbool.h>

typedef enum
{
    INPUT_FILE,
    INPUT_STDIN
} InputMode;

typedef struct Parameters
{
    char *expression;
    InputMode inputMode;
    char *filename;
    int aParameter;
    bool bParameter;
    bool cParameter;
    bool iParameter;
    bool nParameter;
    bool vParameter;
    bool xParameter;
    bool eParameter;
} Parameters;

Parameters* ParseParameters(int argc, char *argv[]);
void FreeParameters(Parameters *parameters);
void InitializeParametersStruct(Parameters *parameters);

#endif
