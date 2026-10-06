#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 100

#define TOKEN_WORD 1
#define TOKEN_PIPE 2
#define TOKEN_INPUT 3
#define TOKEN_OUTPUT 4

typedef struct
{
    char value[MAX_INPUT];
    int type;
} Token;

typedef struct
{
    char command[MAX_INPUT];
    char *arguments[MAX_TOKENS];
    int argument_count;

    char input_file[MAX_INPUT];
    char output_file[MAX_INPUT];

    int has_input;
    int has_output;
} CommandNode;

void print_tree(CommandNode commands[], int count)
{
    printf("\nParse Tree / Execution Structure\n");
    printf("================================\n");

    for (int i = 0; i < count; i++)
    {
        printf("COMMAND %d\n", i + 1);
        printf("|\n");
        printf("+-- Command: %s\n", commands[i].command);

        for (int j = 0; j < commands[i].argument_count; j++)
        {
            printf("|   +-- Argument: %s\n",
                   commands[i].arguments[j]);
        }

        if (commands[i].has_input)
        {
            printf("|   +-- Input: %s\n",
                   commands[i].input_file);
        }

        if (commands[i].has_output)
        {
            printf("|   +-- Output: %s\n",
                   commands[i].output_file);
        }

        if (i < count - 1)
        {
            printf("|\n");
            printf("+-- PIPE |\n");
        }
    }
}

int parse_tokens(Token tokens[],
                 int token_count,
                 CommandNode commands[],
                 int *command_count)
{
    if (token_count == 0)
    {
        printf("Error: Empty command.\n");
        return 0;
    }

    int current = 0;
    int expecting_command = 1;

    memset(commands, 0,
           sizeof(CommandNode) * MAX_TOKENS);

    for (int i = 0; i < token_count; i++)
    {
        Token *token = &tokens[i];

        /* Pipe */
        if (token->type == TOKEN_PIPE)
        {
            if (expecting_command)
            {
                printf("Syntax Error: Invalid pipe placement.\n");
                return 0;
            }

            if (current >= MAX_TOKENS - 1)
            {
                printf("Error: Too many commands.\n");
                return 0;
            }

            current++;
            expecting_command = 1;
            continue;
        }

        /* Command / argument */
        if (token->type == TOKEN_WORD)
        {
            if (expecting_command)
            {
                strncpy(commands[current].command,
                        token->value,
                        MAX_INPUT - 1);

                commands[current].command[MAX_INPUT - 1] = '\0';

                expecting_command = 0;
            }
            else
            {
                if (commands[current].argument_count >=
                    MAX_TOKENS - 1)
                {
                    printf("Error: Too many arguments.\n");
                    return 0;
                }

                commands[current]
                    .arguments[commands[current].argument_count++] =
                    token->value;
            }

            continue;
        }

        /* Input redirection */
        if (token->type == TOKEN_INPUT)
        {
            if (i + 1 >= token_count ||
                tokens[i + 1].type != TOKEN_WORD)
            {
                printf("Syntax Error: Input redirection requires a file.\n");
                return 0;
            }

            if (commands[current].has_input)
            {
                printf("Syntax Error: Multiple input redirections.\n");
                return 0;
            }

            strcpy(commands[current].input_file,
                   tokens[++i].value);

            commands[current].has_input = 1;
            continue;
        }

        /* Output redirection */
        if (token->type == TOKEN_OUTPUT)
        {
            if (i + 1 >= token_count ||
                tokens[i + 1].type != TOKEN_WORD)
            {
                printf("Syntax Error: Output redirection requires a file.\n");
                return 0;
            }

            if (commands[current].has_output)
            {
                printf("Syntax Error: Multiple output redirections.\n");
                return 0;
            }

            strcpy(commands[current].output_file,
                   tokens[++i].value);

            commands[current].has_output = 1;
            continue;
        }
    }

    if (expecting_command)
    {
        printf("Syntax Error: Command expected after pipe.\n");
        return 0;
    }

    *command_count = current + 1;

    return 1;
}

int main(void)
{
    char input[MAX_INPUT];
    Token tokens[MAX_TOKENS];
    CommandNode commands[MAX_TOKENS];

    printf("OSSP Parser\n");
    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        return EXIT_FAILURE;
    }

    /* Empty command */
    if (input[0] == '\n' || input[0] == '\0')
    {
        printf("Empty command. Nothing to parse.\n");
        return EXIT_SUCCESS;
    }

    /*
     * Simple tokenizer inside parser.
     */
    int token_count = 0;
    int i = 0;

    while (input[i] != '\0' &&
           token_count < MAX_TOKENS)
    {
        while (input[i] == ' ' ||
               input[i] == '\t' ||
               input[i] == '\n')
        {
            i++;
        }

        if (input[i] == '\0')
            break;

        if (input[i] == '|')
        {
            strcpy(tokens[token_count].value, "|");
            tokens[token_count].type = TOKEN_PIPE;
            token_count++;
            i++;
            continue;
        }

        if (input[i] == '<')
        {
            strcpy(tokens[token_count].value, "<");
            tokens[token_count].type = TOKEN_INPUT;
            token_count++;
            i++;
            continue;
        }

        if (input[i] == '>')
        {
            strcpy(tokens[token_count].value, ">");
            tokens[token_count].type = TOKEN_OUTPUT;
            token_count++;
            i++;
            continue;
        }

        int j = 0;

        while (input[i] != '\0' &&
               input[i] != ' ' &&
               input[i] != '\t' &&
               input[i] != '\n' &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>')
        {
            if (j < MAX_INPUT - 1)
            {
                tokens[token_count].value[j++] = input[i];
            }

            i++;
        }

        tokens[token_count].value[j] = '\0';
        tokens[token_count].type = TOKEN_WORD;

        token_count++;
    }

    printf("\nTokens detected: %d\n", token_count);

    int command_count = 0;

    if (!parse_tokens(tokens,
                      token_count,
                      commands,
                      &command_count))
    {
        return EXIT_FAILURE;
    }

    print_tree(commands, command_count);

    return EXIT_SUCCESS;
}
