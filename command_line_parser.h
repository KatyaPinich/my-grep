#ifndef COMMAND_LINE_PARSER_H
#define COMMAND_LINE_PARSER_H

typedef enum
{
    INPUT_FILE,
    INPUT_STDIN
} INPUT_MODE;

typedef struct parameters
{
    INPUT_MODE inputMode;
    char* filename;
} parameters_t;

parameters_t* ParseParameters(int argc, char *argv[]);
void FreeParameters(parameters_t *parameters);

#endif
