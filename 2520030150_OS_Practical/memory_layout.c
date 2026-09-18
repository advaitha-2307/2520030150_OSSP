#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global variable
int global_variable = 100;

// Global uninitialized variable
int global_uninitialized;

// Static global variable
static int static_global = 200;

// Function = code segment
void show_code_address()
{
    printf("Code address (function):       %p\n",
           (void *)show_code_address);
}

int main()
{
    // Local variable = stack
    int stack_variable = 300;

    // Static local variable
    static int static_local = 400;

    // Dynamically allocated variable = heap
    int *heap_variable = malloc(sizeof(int));

    if (heap_variable == NULL)
    {
        perror("malloc");
        return 1;
    }

    *heap_variable = 500;

    printf("============================================\n");
    printf("       LINUX PROCESS MEMORY LAYOUT          \n");
    printf("============================================\n\n");

    // Code
    printf("Code address (function):       %p\n",
           (void *)show_code_address);

    // Global
    printf("Global variable address:       %p\n",
           (void *)&global_variable);

    // Global uninitialized
    printf("Global uninitialized address:  %p\n",
           (void *)&global_uninitialized);

    // Static global
    printf("Static global address:          %p\n",
           (void *)&static_global);

    // Static local
    printf("Static local address:           %p\n",
           (void *)&static_local);

    // Heap
    printf("Heap variable address:          %p\n",
           (void *)heap_variable);

    // Stack
    printf("Stack variable address:         %p\n",
           (void *)&stack_variable);

    printf("\n============================================\n");
    printf("Process ID (PID): %d\n", getpid());
    printf("============================================\n");

    printf("\nProcess is running...\n");
    printf("Use another terminal to inspect /proc/%d/maps\n",
           getpid());

    printf("\nPress ENTER to terminate...\n");
    getchar();

    free(heap_variable);

    return 0;
}
