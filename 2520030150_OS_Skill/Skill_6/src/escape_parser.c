#include <stdio.h>
#include <string.h>

#define MAX_INPUT 1024
#define MAX_TOKEN 256

void parse_input(const char *input) {
    char token[MAX_TOKEN];
    int token_index = 0;
    int escaped = 0;
    int token_count = 0;

    printf("\nParsed Tokens:\n");

    for (int i = 0; input[i] != '\0'; i++) {
        char ch = input[i];

        if (escaped) {
            token[token_index++] = ch;
            escaped = 0;
            continue;
        }

        if (ch == '\\') {
            escaped = 1;
            continue;
        }

        if (ch == ' ' || ch == '\t') {
            if (token_index > 0) {
                token[token_index] = '\0';
                printf("Token %d: [%s]\n", ++token_count, token);
                token_index = 0;
            }
        } else {
            token[token_index++] = ch;
        }
    }

    if (escaped) {
        token[token_index++] = '\\';
    }

    if (token_index > 0) {
        token[token_index] = '\0';
        printf("Token %d: [%s]\n", ++token_count, token);
    }

    printf("Total Tokens: %d\n", token_count);
}

int main(void) {
    char input[MAX_INPUT];

    printf("Escape Sequence Parser\n");
    printf("Enter a command: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    parse_input(input);

    return 0;
}
