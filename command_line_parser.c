#include "command_line_parser.h"

#include <stdlib.h>
#include <stdio.h>

#include "string_tools.h"

Parameters* ParseParameters(int argc, char *argv[])
{
    Parameters *parameters;

    parameters = (Parameters*)malloc(sizeof(Parameters));
    if (parameters == NULL)
        return parameters; // TODO: Maybe exit with a warning memory allocation failed

    if (argc == 2)
    {
        // We have only an expression and get the input from stdin
        parameters->expression = CopyString(argv[1]);
        parameters->inputMode = INPUT_STDIN;
    }
    else if (argc == 3)
    {
        // We have an expression and a file
        parameters->expression = CopyString(argv[1]);
        parameters->inputMode = INPUT_FILE;
        parameters->filename = CopyString(argv[2]);
        if (parameters->filename == NULL)
        {
            FreeParameters(parameters);
            exit(1);
        }
    }
    else
    {
        printf("usage: my-grep [-bcinvx] [-A num] [-E pattern] [pattern] [file]\n");
        FreeParameters(parameters);
        exit(1);
    }

    return parameters;
}

void FreeParameters(Parameters *parameters)
{
    if (parameters->expression != NULL)
    {
        free(parameters->expression);
    }

    if (parameters->inputMode == INPUT_FILE && parameters->filename != NULL)
    {
        free(parameters->filename);
    }

    free(parameters);
}