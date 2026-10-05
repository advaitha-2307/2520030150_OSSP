CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGETS = bin/escape_parser bin/execute_command

all: $(TARGETS)

bin/escape_parser: src/escape_parser.c
	$(CC) $(CFLAGS) src/escape_parser.c -o bin/escape_parser

bin/execute_command: src/execute_command.c
	$(CC) $(CFLAGS) src/execute_command.c -o bin/execute_command

clean:
	rm -f $(TARGETS)

run_escape:
	./bin/escape_parser

run_execute:
	./bin/execute_command
