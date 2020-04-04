#include <stdio.h>

#include "CommandLineParser.h"

int main(int argc, char *argv[])
{
    parameters_t *parameters;

    parameters = ParseParameters(argc, argv);

    printf("\n");

    return 0;
}


