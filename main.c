#include <stdio.h>

#include "command_line_parser.h"

int main(int argc, char *argv[])
{
    Parameters *parameters;

    parameters = ParseParameters(argc, argv);

    printf("\n");

    return 0;
}


