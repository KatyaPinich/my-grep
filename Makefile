my_grep: main.o string_tools.o command_line_parser.o
	gcc -o my_grep main.o string_tools.o command_line_parser.o
	
main.o: main.c command_line_parser.h
	gcc -c main.c 
	
string_tools.o: string_tools.c string_tools.h
	gcc -c string_tools.c 
	
command_line_parser.o: command_line_parser.c command_line_parser.h string_tools.h
	gcc -c command_line_parser.c
	
.PHONY: all clean test

clean:
	-rm *.o my_grep
	
all: my_grep

test: all
	./beta1.sh