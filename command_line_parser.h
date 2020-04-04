#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

typedef enum
{
    INPUT_FILE,
    INPUT_STDIN
} InputMode;

typedef struct Parameters
{
    InputMode inputMode;
    char* filename;
} Parameters;

Parameters* ParseParameters(int argc, char *argv[]);
void FreeParameters(Parameters *parameters);

#endif
