#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *command;
    struct Node *next;
} Node;

/* Dynamically add a command to the linked list */
void add_node(Node **head, const char *command)
{
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->command = malloc(strlen(command) + 1);

    if (new_node->command == NULL)
    {
        perror("malloc");
        free(new_node);
        exit(EXIT_FAILURE);
    }

    strcpy(new_node->command, command);

    new_node->next = *head;
    *head = new_node;
}

/* Display linked list */
void display_nodes(Node *head)
{
    Node *current = head;

    printf("\nStored commands:\n");

    while (current != NULL)
    {
        printf("- %s\n", current->command);
        current = current->next;
    }
}

/* Free all allocated memory */
void free_nodes(Node **head)
{
    Node *current = *head;

    while (current != NULL)
    {
        Node *next = current->next;

        free(current->command);
        free(current);

        current = next;
    }

    *head = NULL;
}

int main(void)
{
    size_t capacity = 4;
    size_t count = 0;

    char **commands = malloc(capacity * sizeof(char *));

    if (commands == NULL)
    {
        perror("malloc");
        return EXIT_FAILURE;
    }

    printf("Dynamic Memory Demonstration\n");

    const char *input[] =
    {
        "ls",
        "pwd",
        "gcc program.c",
        "make",
        "exit"
    };

    size_t input_count = sizeof(input) / sizeof(input[0]);

    for (size_t i = 0; i < input_count; i++)
    {
        if (count >= capacity)
        {
            capacity *= 2;

            char **temp = realloc(commands,
                                  capacity * sizeof(char *));

            if (temp == NULL)
            {
                perror("realloc");
                free(commands);
                return EXIT_FAILURE;
            }

            commands = temp;

            printf("Buffer resized. New capacity: %zu\n", capacity);
        }

        commands[count] = malloc(strlen(input[i]) + 1);

        if (commands[count] == NULL)
        {
            perror("malloc");

            for (size_t j = 0; j < count; j++)
                free(commands[j]);

            free(commands);

            return EXIT_FAILURE;
        }

        strcpy(commands[count], input[i]);

        printf("Allocated: %s\n", commands[count]);

        count++;
    }

    printf("\nDynamic array contents:\n");

    for (size_t i = 0; i < count; i++)
    {
        printf("%zu: %s\n", i + 1, commands[i]);
    }

    /* Linked list demonstration */

    Node *head = NULL;

    add_node(&head, "ls");
    add_node(&head, "pwd");
    add_node(&head, "history");

    display_nodes(head);

    /* Release linked-list memory */
    free_nodes(&head);

    /* Release dynamic array memory */
    for (size_t i = 0; i < count; i++)
    {
        free(commands[i]);
    }

    free(commands);

    printf("\nAll allocated memory released successfully.\n");

    return 0;
}
