#include "CommandLineParser.h"

#include <stdlib.h>
#include <stdio.h>

#include "StringTools.h"

parameters_t* ParseParameters(int argc, char *argv[])
{
    parameters_t *parameters;

    parameters = (parameters_t*)malloc(sizeof(parameters_t));
    if (parameters == NULL)
        return parameters; // TODO: Maybe exit with a warning memory allocation failed

    if (argc == 2)
    {
        // We have only an expression and get the input from stdin
        parameters->inputMode = INPUT_STDIN;
    }
    else if (argc == 3)
    {
        // We have an expression and a file
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

void FreeParameters(parameters_t *parameters)
{
    if (parameters->inputMode == INPUT_FILE && parameters->filename != NULL)
    {
        free(parameters->filename);
    }

    free(parameters);
}