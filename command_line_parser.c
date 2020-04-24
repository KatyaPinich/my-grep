#include "command_line_parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "string_tools.h"

#define USAGE_MESSAGE "usage: my-grep [-bcinvx] [-A num] [-E pattern] [pattern] [file]"

bool GetOption(char *argv[], int option_index, char *option);
bool HasArguments(char option);
bool ParseOption(Parameters *parameters, char option, int option_argument, char *argv[]);
bool ParseNonOptionArguments(Parameters *parameters, int arg_index, char *argv[]);
void InitializeParametersStruct(Parameters *parameters);
void PrintErrorAndExit(const char *error_message, Parameters *parameters);

Parameters* ParseParameters(int argc, char *argv[])
{
    Parameters *parameters;
    int arg_index;
    char option;
    int option_argument;

    parameters = (Parameters*)malloc(sizeof(Parameters));
    if (parameters == NULL)
        return parameters; // TODO: Maybe exit with a warning memory allocation failed

    InitializeParametersStruct(parameters);

    arg_index = 1;
    while (arg_index < argc)
    {
        if (GetOption(argv, arg_index, &option))
        {
            if (HasArguments(option))
            {
                arg_index++;
                option_argument = arg_index;
            }

            if (!ParseOption(parameters, option, option_argument, argv))
            {
                PrintErrorAndExit(USAGE_MESSAGE, parameters);
            }
        }
        else
        {
            if (!ParseNonOptionArguments(parameters, arg_index, argv))
            {
                PrintErrorAndExit(USAGE_MESSAGE, parameters);
            }
        }

        arg_index++;
    }

    if (parameters->expression == NULL)
    {
        PrintErrorAndExit(USAGE_MESSAGE, parameters);
    }

    return parameters;
}

bool GetOption(char *argv[], int option_index, char *option)
{
    bool option_found = false;

    if (argv[option_index][0] == '-')
    {
        option_found = true;
        *option = argv[option_index][1];
    }

    return option_found;
}

bool HasArguments(char option)
{
    switch (option) {
        case 'A':
        case 'E':
            return true;
        default:
            return false;
    }
}

bool ParseOption(Parameters *parameters, char option, int option_argument, char *argv[])
{
    bool valid_option = true;

    switch (option) {
        case 'A':
            parameters->aParameter = atoi(argv[option_argument]); // TODO: Check parsing
            break;
        case 'b':
            parameters->bParameter = true;
            break;
        case 'c':
            parameters->cParameter = true;
            break;
        case 'i':
            parameters->ignoreCase = true;
            break;
        case 'n':
            parameters->nParameter = true;
            break;
        case 'v':
            parameters->vParameter = true;
            break;
        case 'E':
            parameters->eParameter = true;
            parameters->expression = CopyString(argv[option_argument]);
            break;
        default:
            valid_option = false;
            break;
    }

    return valid_option;
}

bool ParseNonOptionArguments(Parameters *parameters, int arg_index, char *argv[])
{
    bool valid_argument = true;

    if (!parameters->eParameter && parameters->expression == NULL)
    {
        parameters->expression = CopyString(argv[arg_index]);
        if (parameters->expression == NULL)
        {
            FreeParameters(parameters);
            exit(EXIT_FAILURE);
        }
    }
    else if (parameters->filename == NULL)
    {
        parameters->inputMode = INPUT_FILE;
        parameters->filename = CopyString(argv[arg_index]);
        if (parameters->filename == NULL)
        {
            FreeParameters(parameters);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        valid_argument = false;
    }

    return valid_argument;
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
    parameters->ignoreCase = false;
    parameters->nParameter = false;
    parameters->vParameter = false;
    parameters->xParameter = false;
}

void PrintErrorAndExit(const char *error_message, Parameters *parameters)
{
    fprintf(stderr, "%s", error_message);
    FreeParameters(parameters);
    exit(EXIT_FAILURE);
}