cmake_minimum_required(VERSION 3.15)
project(my_grep C)

set(CMAKE_C_STANDARD 99)

add_executable(my_grep main.c string_tools.c string_tools.h command_line_parser.c command_line_parser.h linked_list.c linked_list.h stream_handler.h stream_handler.c regular_expression.c regular_expression.h match_reporter.c match_reporter.h match_finder.c match_finder.h)