#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 100

typedef struct
{
    char value[MAX_INPUT];
    int type;
} Token;

/* Token types */
#define TOKEN_WORD 1
#define TOKEN_PIPE 2
#define TOKEN_INPUT 3
#define TOKEN_OUTPUT 4

void print_token_type(int type)
{
    switch (type)
    {
        case TOKEN_WORD:
            printf("WORD");
            break;

        case TOKEN_PIPE:
            printf("PIPE");
            break;

        case TOKEN_INPUT:
            printf("INPUT_REDIRECTION");
            break;

        case TOKEN_OUTPUT:
            printf("OUTPUT_REDIRECTION");
            break;

        default:
            printf("UNKNOWN");
    }
}

int tokenize(const char *input, Token tokens[])
{
    int count = 0;
    int i = 0;

    while (input[i] != '\0' && count < MAX_TOKENS)
    {
        /* Skip whitespace */
        while (isspace((unsigned char)input[i]))
        {
            i++;
        }

        if (input[i] == '\0')
            break;

        /* Pipe */
        if (input[i] == '|')
        {
            strcpy(tokens[count].value, "|");
            tokens[count].type = TOKEN_PIPE;
            count++;
            i++;
            continue;
        }

        /* Input redirection */
        if (input[i] == '<')
        {
            strcpy(tokens[count].value, "<");
            tokens[count].type = TOKEN_INPUT;
            count++;
            i++;
            continue;
        }

        /* Output redirection */
        if (input[i] == '>')
        {
            strcpy(tokens[count].value, ">");
            tokens[count].type = TOKEN_OUTPUT;
            count++;
            i++;
            continue;
        }

        /* Normal word */
        int j = 0;

        while (input[i] != '\0' &&
               !isspace((unsigned char)input[i]) &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>')
        {
            if (j < MAX_INPUT - 1)
            {
                tokens[count].value[j++] = input[i];
            }

            i++;
        }

        tokens[count].value[j] = '\0';
        tokens[count].type = TOKEN_WORD;
        count++;
    }

    return count;
}

int main(void)
{
    char input[MAX_INPUT];
    Token tokens[MAX_TOKENS];

    printf("OSSP Tokenizer\n");
    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        return EXIT_FAILURE;
    }

    int count = tokenize(input, tokens);

    printf("\nToken Stream:\n");

    for (int i = 0; i < count; i++)
    {
        printf("Token %d: ", i + 1);

        print_token_type(tokens[i].type);

        printf(" -> [%s]\n", tokens[i].value);
    }

    printf("\nTotal tokens: %d\n", count);

    return EXIT_SUCCESS;
}
