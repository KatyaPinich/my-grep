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
    int lines_after_context;
    bool byte_offset;
    bool print_line_count;
    bool ignore_case;
    bool line_number;
    bool invert_match;
    bool exact_match;
    bool is_regexp;
} Parameters;

Parameters* ParseParameters(int argc, char *argv[]);
void FreeParameters(Parameters *parameters);

#endif
