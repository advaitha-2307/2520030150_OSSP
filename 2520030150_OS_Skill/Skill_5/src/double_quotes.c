#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_TOKENS 100

typedef struct
{
    char value[MAX_INPUT];
    int quoted;
} Token;

void expand_variable(char *output, const char *input)
{
    int i = 0;
    int j = 0;

    while (input[i] != '\0' && j < MAX_INPUT - 1)
    {
        if (input[i] == '$')
        {
            i++;

            char variable[128];
            int k = 0;

            while ((input[i] >= 'A' && input[i] <= 'Z') ||
                   (input[i] >= 'a' && input[i] <= 'z') ||
                   (input[i] >= '0' && input[i] <= '9') ||
                   input[i] == '_')
            {
                if (k < (int)sizeof(variable) - 1)
                {
                    variable[k++] = input[i];
                }

                i++;
            }

            variable[k] = '\0';

            const char *value = getenv(variable);

            if (value != NULL)
            {
                while (*value != '\0' &&
                       j < MAX_INPUT - 1)
                {
                    output[j++] = *value++;
                }
            }
        }
        else
        {
            output[j++] = input[i++];
        }
    }

    output[j] = '\0';
}

int parse_double_quotes(const char *input, Token tokens[])
{
    int i = 0;
    int count = 0;

    while (input[i] != '\0' && count < MAX_TOKENS)
    {
        while (input[i] == ' ' ||
               input[i] == '\t' ||
               input[i] == '\n')
        {
            i++;
        }

        if (input[i] == '\0')
            break;

        int j = 0;

        /* Double quoted string */
        if (input[i] == '"')
        {
            char quoted_content[MAX_INPUT];

            i++;

            while (input[i] != '\0' && input[i] != '"')
            {
                if (j < MAX_INPUT - 1)
                {
                    quoted_content[j++] = input[i];
                }

                i++;
            }

            if (input[i] != '"')
            {
                printf("Syntax Error: Unclosed double quote.\n");
                return -1;
            }

            quoted_content[j] = '\0';
            i++;

            tokens[count].quoted = 1;

            expand_variable(tokens[count].value,
                            quoted_content);
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

            tokens[count].value[j] = '\0';
        }

        count++;
    }

    return count;
}

int main(void)
{
    char input[MAX_INPUT];
    Token tokens[MAX_TOKENS];

    printf("Double Quote Parser\n");
    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
        return EXIT_FAILURE;

    int count = parse_double_quotes(input, tokens);

    if (count < 0)
        return EXIT_FAILURE;

    printf("\nParsed Tokens:\n");

    for (int i = 0; i < count; i++)
    {
        printf("Token %d: [%s]", i + 1, tokens[i].value);

        if (tokens[i].quoted)
            printf("  (double quoted)");

        printf("\n");
    }

    return EXIT_SUCCESS;
}
