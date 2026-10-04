CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = bin/shell_input
SOURCE = src/shell_input.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
