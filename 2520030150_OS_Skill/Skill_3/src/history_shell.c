#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 1024
#define HISTORY_SIZE 20

static struct termios original_terminal;

char *history[HISTORY_SIZE];
int history_count = 0;
int history_position = 0;

void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal);
}

void enable_raw_mode(void)
{
    struct termios raw;

    tcgetattr(STDIN_FILENO, &original_terminal);

    atexit(disable_raw_mode);

    raw = original_terminal;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void display_prompt(void)
{
    printf("ossp-history> ");
    fflush(stdout);
}

void clear_input(char *buffer, int *position)
{
    while (*position > 0)
    {
        printf("\b \b");
        (*position)--;
    }

    buffer[0] = '\0';
}

void add_history(const char *command)
{
    if (strlen(command) == 0)
        return;

    if (history_count == HISTORY_SIZE)
    {
        free(history[0]);

        for (int i = 1; i < HISTORY_SIZE; i++)
        {
            history[i - 1] = history[i];
        }

        history_count--;
    }

    history[history_count] = malloc(strlen(command) + 1);

    if (history[history_count] == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(history[history_count], command);
    history_count++;
}

void show_history(void)
{
    printf("\n\nCommand History:\n");

    for (int i = 0; i < history_count; i++)
    {
        printf("%d  %s\n", i + 1, history[i]);
    }
}

int read_command(char *buffer, int size)
{
    int position = 0;
    char c;

    history_position = history_count;

    while (position < size - 1)
    {
        if (read(STDIN_FILENO, &c, 1) != 1)
            return -1;

        /* Enter */
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

        /* Escape sequence */
        if (c == 27)
        {
            char seq[2];

            if (read(STDIN_FILENO, &seq[0], 1) != 1)
                continue;

            if (read(STDIN_FILENO, &seq[1], 1) != 1)
                continue;

            /* Up arrow */
            if (seq[0] == '[' && seq[1] == 'A')
            {
                if (history_position > 0)
                {
                    history_position--;

                    clear_input(buffer, &position);

                    strcpy(buffer, history[history_position]);
                    position = strlen(buffer);

                    printf("%s", buffer);
                    fflush(stdout);
                }

                continue;
            }

            /* Down arrow */
            if (seq[0] == '[' && seq[1] == 'B')
            {
                if (history_position < history_count - 1)
                {
                    history_position++;

                    clear_input(buffer, &position);

                    strcpy(buffer, history[history_position]);
                    position = strlen(buffer);

                    printf("%s", buffer);
                    fflush(stdout);
                }
                else
                {
                    history_position = history_count;

                    clear_input(buffer, &position);
                }

                continue;
            }

            continue;
        }

        /* Normal characters */
        if (c >= 32 && c <= 126)
        {
            buffer[position] = c;
            position++;

            putchar(c);
            fflush(stdout);
        }
    }

    buffer[position] = '\0';

    return position;
}

void free_history(void)
{
    for (int i = 0; i < history_count; i++)
    {
        free(history[i]);
        history[i] = NULL;
    }

    history_count = 0;
}

int main(void)
{
    char input[BUFFER_SIZE];

    enable_raw_mode();

    while (1)
    {
        display_prompt();

        if (read_command(input, BUFFER_SIZE) < 0)
            break;

        if (strlen(input) == 0)
            continue;

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting history shell...\n");
            break;
        }

        if (strcmp(input, "history") == 0)
        {
            show_history();
            continue;
        }

        add_history(input);

        printf("Command stored: %s\n", input);
    }

    free_history();

    return 0;
}
