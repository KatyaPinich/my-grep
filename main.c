#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
char* CopyString(const char *sourceString);
void FreeParameters(parameters_t *parameters);

int main(int argc, char *argv[])
{
    parameters_t *parameters;

    parameters = ParseParameters(argc, argv);

    return 0;
}

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

char* CopyString(const char *sourceString)
{
    char *destinationString;
    int sourceStringLength;

    sourceStringLength = strlen(sourceString) + 1;
    destinationString = (char*)malloc(sizeof(char) * sourceStringLength);
    if (destinationString == NULL)
        return NULL;

    strncpy(destinationString, sourceString, sourceStringLength);
    return destinationString;
}
