my_grep: main.o string_tools.o command_line_parser.o linked_list.o regular_expression.o stream_handler.o match_finder.o match_reporter.o
	gcc -o my_grep main.o string_tools.o command_line_parser.o linked_list.o regular_expression.o stream_handler.o match_finder.o match_reporter.o
	
main.o: main.c command_line_parser.h linked_list.h string_tools.h stream_handler.h regular_expression.h match_finder.h match_reporter.h
	gcc -c main.c 
	
string_tools.o: string_tools.c string_tools.h 
	gcc -c string_tools.c 
	
command_line_parser.o: command_line_parser.c command_line_parser.h string_tools.h
	gcc -c command_line_parser.c
	
linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c
	
regular_expression.o: regular_expression.c regular_expression.h string_tools.h
	gcc -c regular_expression.c
	
stream_handler.o: stream_handler.c stream_handler.h command_line_parser.h
	gcc -c stream_handler.c
	
match_finder.o: match_finder.c match_finder.h regular_expression.h
	gcc -c match_finder.c
	
match_reporter.o: match_reporter.c match_reporter.h linked_list.h command_line_parser.h
	gcc -c match_reporter.c

.PHONY: all clean test

clean:
	-rm *.o my_grep
	
all: my_grep

test: all
	./run_all.sh