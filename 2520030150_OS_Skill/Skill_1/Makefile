CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: fork_demo exec_demo fork_exec

fork_demo:
	$(CC) $(CFLAGS) src/fork_demo.c -o bin/fork_demo

exec_demo:
	$(CC) $(CFLAGS) src/exec_demo.c -o bin/exec_demo

fork_exec:
	$(CC) $(CFLAGS) src/fork_exec.c -o bin/fork_exec

clean:
	rm -f bin/*
