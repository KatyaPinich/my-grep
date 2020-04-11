#include "command_line_parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_tools.h"

Parameters* ParseParameters(int argc, char *argv[])
{
    Parameters *parameters;
    int i;

    parameters = (Parameters*)malloc(sizeof(Parameters));
    if (parameters == NULL)
        return parameters; // TODO: Maybe exit with a warning memory allocation failed

    InitializeParametersStruct(parameters);

    for(i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-A") == 0)
        {
            parameters->aParameter = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            parameters->bParameter = true;
        }
        else if (strcmp(argv[i], "-c") == 0)
        {
            parameters->cParameter = true;
        }
        else if (strcmp(argv[i], "-i") == 0)
        {
            parameters->iParameter = true;
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            parameters->nParameter = true;
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            parameters->vParameter = true;
        }
        else if (strcmp(argv[i], "-E") == 0)
        {
            parameters->eParameter = true;
            parameters->expression = CopyString(argv[i + 1]);
            i++;
        }
        else if (parameters->expression == NULL)
        {
            parameters->expression = CopyString(argv[i]);
            if (parameters->expression == NULL)
            {
                FreeParameters(parameters);
                exit(1);
            }
        }
        else if (parameters->filename == NULL)
        {
            parameters->inputMode = INPUT_FILE;
            parameters->filename = CopyString(argv[i]);
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

void InitializeParametersStruct(Parameters *parameters)
{
    parameters->filename = NULL;
    parameters->expression = NULL;
    parameters->inputMode = INPUT_STDIN;
    parameters->aParameter = -1;
    parameters->bParameter = false;
    parameters->cParameter = false;
    parameters->eParameter = false;
    parameters->iParameter = false;
    parameters->nParameter = false;
    parameters->vParameter = false;
    parameters->xParameter = false;
}