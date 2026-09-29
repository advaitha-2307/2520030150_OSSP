#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTS 1000000

long long counter = 0;

pthread_mutex_t counter_mutex =
    PTHREAD_MUTEX_INITIALIZER;

void *increment_counter(void *arg)
{
    for (long long i = 0; i < INCREMENTS; i++)
    {
        pthread_mutex_lock(&counter_mutex);

        counter++;

        pthread_mutex_unlock(&counter_mutex);
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];

    printf("Starting mutex-protected test...\n");

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
        if (pthread_join(
                threads[i],
                NULL) != 0)
        {
            perror("pthread_join");
            return 1;
        }
    }

    printf("\nExpected counter value : %lld\n",
           (long long)NUM_THREADS * INCREMENTS);

    printf("Actual counter value   : %lld\n",
           counter);

    pthread_mutex_destroy(&counter_mutex);

    return 0;
}
