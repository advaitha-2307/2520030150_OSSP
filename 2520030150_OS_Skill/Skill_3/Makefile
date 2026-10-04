CC = gcc
CFLAGS = -Wall -Wextra -std=c11

HISTORY_SRC = src/history_shell.c
HISTORY_BIN = bin/history_shell

MEMORY_SRC = src/dynamic_memory.c
MEMORY_BIN = bin/dynamic_memory

all: $(HISTORY_BIN) $(MEMORY_BIN)

$(HISTORY_BIN): $(HISTORY_SRC)
	$(CC) $(CFLAGS) $(HISTORY_SRC) -o $(HISTORY_BIN)

$(MEMORY_BIN): $(MEMORY_SRC)
	$(CC) $(CFLAGS) $(MEMORY_SRC) -o $(MEMORY_BIN)

run-history: $(HISTORY_BIN)
	./$(HISTORY_BIN)

run-memory: $(MEMORY_BIN)
	./$(MEMORY_BIN)

valgrind: $(MEMORY_BIN)
	valgrind --leak-check=full ./$(MEMORY_BIN)

clean:
	rm -f $(HISTORY_BIN) $(MEMORY_BIN)
