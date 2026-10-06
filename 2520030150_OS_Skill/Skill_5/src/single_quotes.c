#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 100

typedef struct
{
    char value[MAX_INPUT];
    int quoted;
} Token;

int parse_single_quotes(const char *input, Token tokens[])
{
    int i = 0;
    int count = 0;

    while (input[i] != '\0' && count < MAX_TOKENS)
    {
        /* Skip whitespace */
        while (input[i] == ' ' ||
               input[i] == '\t' ||
               input[i] == '\n')
        {
            i++;
        }

        if (input[i] == '\0')
            break;

        int j = 0;

        /* Single quoted string */
        if (input[i] == '\'')
        {
            tokens[count].quoted = 1;
            i++;

            while (input[i] != '\0' && input[i] != '\'')
            {
                if (j < MAX_INPUT - 1)
                {
                    tokens[count].value[j++] = input[i];
                }

                i++;
            }

            if (input[i] != '\'')
            {
                printf("Syntax Error: Unclosed single quote.\n");
                return -1;
            }

            i++;
        }
        else
        {
            tokens[count].quoted = 0;

            while (input[i] != '\0' &&
                   input[i] != ' ' &&
                   input[i] != '\t' &&
                   input[i] != '\n')
            {
                if (j < MAX_INPUT - 1)
                {
                    tokens[count].value[j++] = input[i];
                }

                i++;
            }
        }

        tokens[count].value[j] = '\0';
        count++;
    }

    return count;
}

int main(void)
{
    char input[MAX_INPUT];
    Token tokens[MAX_TOKENS];

    printf("Single Quote Parser\n");
    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
        return EXIT_FAILURE;

    int count = parse_single_quotes(input, tokens);

    if (count < 0)
        return EXIT_FAILURE;

    printf("\nParsed Tokens:\n");

    for (int i = 0; i < count; i++)
    {
        printf("Token %d: [%s]", i + 1, tokens[i].value);

        if (tokens[i].quoted)
            printf("  (single quoted)");

        printf("\n");
    }

    return EXIT_SUCCESS;
}
