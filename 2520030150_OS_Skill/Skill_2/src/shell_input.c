#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 1024

static struct termios original_terminal;

/* Restore normal terminal settings */
void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
}

/* Enable character-by-character keyboard input */
void enable_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &original_terminal);

    atexit(disable_raw_mode);

    raw = original_terminal;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* Display shell prompt */
void display_prompt(void)
{
    printf("ossp-shell> ");
    fflush(stdout);
}

/* Read keyboard input */
int read_command(char *buffer, int size)
{
    int position = 0;
    char c;

    while (position < size - 1)
    {
        if (read(STDIN_FILENO, &c, 1) != 1)
        {
            return -1;
        }

        /* Enter key */
        if (c == '\n' || c == '\r')
        {
            buffer[position] = '\0';
            printf("\n");
            return position;
        }

        /* Backspace */
        if (c == 127 || c == '\b')
        {
            if (position > 0)
            {
                position--;
                buffer[position] = '\0';

                printf("\b \b");
                fflush(stdout);
            }

            continue;
        }

        /* Normal printable character */
        if (c >= 32 && c <= 126)
        {
            buffer[position] = c;
            position++;

            putchar(c);
            fflush(stdout);
        }
    }

    buffer[position] = '\0';
    printf("\n");

    return position;
}

int main(void)
{
    char input[BUFFER_SIZE];

    enable_raw_mode();

    /* Main interactive loop */
    while (1)
    {
        /* Display prompt */
        display_prompt();

        /* Read input */
        if (read_command(input, BUFFER_SIZE) < 0)
        {
            break;
        }

        /* Handle empty input */
        if (strlen(input) == 0)
        {
            continue;
        }

        /* Handle exit command */
        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting OSSP shell...\n");
            break;
        }

        /* Display received command */
        printf("Command received: %s\n", input);
    }

    return 0;
}
