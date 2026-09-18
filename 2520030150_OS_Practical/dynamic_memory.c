#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i;

    printf("====================================\n");
    printf("     DYNAMIC MEMORY ALLOCATION      \n");
    printf("====================================\n\n");

    /*
     * ------------------------------------------------
     * 1. malloc()
     * ------------------------------------------------
     */

    printf("1. malloc() demonstration\n");

    int *arr1 = malloc(5 * sizeof(int));

    if (arr1 == NULL)
    {
        printf("malloc failed\n");
        return 1;
    }

    for (i = 0; i < 5; i++)
    {
        arr1[i] = (i + 1) * 10;
    }

    printf("Values allocated using malloc:\n");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", arr1[i]);
    }

    printf("\n\n");


    /*
     * ------------------------------------------------
     * 2. calloc()
     * ------------------------------------------------
     */

    printf("2. calloc() demonstration\n");

    int *arr2 = calloc(5, sizeof(int));

    if (arr2 == NULL)
    {
        printf("calloc failed\n");
        free(arr1);
        return 1;
    }

    printf("Values immediately after calloc:\n");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", arr2[i]);
    }

    printf("\n");

    for (i = 0; i < 5; i++)
    {
        arr2[i] = (i + 1) * 100;
    }

    printf("Values after assigning data:\n");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", arr2[i]);
    }

    printf("\n\n");


    /*
     * ------------------------------------------------
     * 3. realloc()
     * ------------------------------------------------
     */

    printf("3. realloc() demonstration\n");

    printf("Original arr1 size: 5 integers\n");

    int *temp = realloc(arr1, 10 * sizeof(int));

    if (temp == NULL)
    {
        printf("realloc failed\n");

        free(arr1);
        free(arr2);

        return 1;
    }

    arr1 = temp;

    for (i = 5; i < 10; i++)
    {
        arr1[i] = (i + 1) * 10;
    }

    printf("After realloc, array contains:\n");

    for (i = 0; i < 10; i++)
    {
        printf("%d ", arr1[i]);
    }

    printf("\n\n");


    /*
     * ------------------------------------------------
     * 4. free()
     * ------------------------------------------------
     */

    printf("4. free() demonstration\n");

    free(arr1);
    printf("arr1 memory released.\n");

    free(arr2);
    printf("arr2 memory released.\n");

    printf("\nAll dynamically allocated memory was freed.\n");

    return 0;
}
