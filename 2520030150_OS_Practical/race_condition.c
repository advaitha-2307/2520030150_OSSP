#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS 1000000

long long counter = 0;

void *increment_counter(void *arg)
{
    for (long long i = 0; i < INCREMENTS; i++)
    {
        counter++;
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    printf("Starting race-condition test...\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_create(
                &threads[i],
                NULL,
                increment_counter,
                NULL) != 0)
        {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            return 1;
        }
    }

    printf("\nExpected counter value : %lld\n",
           (long long)NUM_THREADS * INCREMENTS);

    printf("Actual counter value   : %lld\n",
           counter);

    return 0;
}
