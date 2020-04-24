#ifndef STREAM_HANDLER_H
#define STREAM_HANDLER_H

#include <stdio.h>

#include "command_line_parser.h"

FILE* GetInputStream(Parameters *parameters);
FILE* OpenFile(const char *filename);
char* ReadLine(FILE* input_stream);

#endif
