#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 100000

int *buffer;
int buffer_size;

int in = 0;
int out = 0;

sem_t empty;
sem_t full;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

long long produced_count = 0;
long long consumed_count = 0;

void *producer(void *arg)
{
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        int item = i;

        /*
         * Wait until at least one buffer slot is empty.
         */
        sem_wait(&empty);

        /*
         * Enter critical section.
         */
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        in = (in + 1) % buffer_size;

        produced_count++;

        /*
         * Leave critical section.
         */
        pthread_mutex_unlock(&mutex);

        /*
         * One more item is now available.
         */
        sem_post(&full);
    }

    return NULL;
}

void *consumer(void *arg)
{
    int total_items =
        (NUM_PRODUCERS * ITEMS_PER_PRODUCER) /
        NUM_CONSUMERS;

    for (int i = 0; i < total_items; i++)
    {
        int item;

        /*
         * Wait until an item is available.
         */
        sem_wait(&full);

        /*
         * Enter critical section.
         */
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        out = (out + 1) % buffer_size;

        consumed_count++;

        /*
         * Leave critical section.
         */
        pthread_mutex_unlock(&mutex);

        /*
         * One buffer slot is now free.
         */
        sem_post(&empty);

        (void)item;
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <buffer_size>\n", argv[0]);
        return 1;
    }

    buffer_size = atoi(argv[1]);

    if (buffer_size <= 0)
    {
        printf("Buffer size must be greater than 0.\n");
        return 1;
    }

    buffer = malloc(
        buffer_size * sizeof(int)
    );

    if (buffer == NULL)
    {
        perror("malloc");
        return 1;
    }

    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    /*
     * Initially every buffer slot is empty.
     */
    sem_init(
        &empty,
        0,
        buffer_size
    );

    /*
     * Initially there are no produced items.
     */
    sem_init(
        &full,
        0,
        0
    );

    produced_count = 0;
    consumed_count = 0;

    struct timespec start, end;

    clock_gettime(
        CLOCK_MONOTONIC,
        &start
    );

    /*
     * Create producer threads.
     */
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_create(
            &producers[i],
            NULL,
            producer,
            NULL
        );
    }

    /*
     * Create consumer threads.
     */
    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_create(
            &consumers[i],
            NULL,
            consumer,
            NULL
        );
    }

    /*
     * Wait for producers.
     */
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(
            producers[i],
            NULL
        );
    }

    /*
     * Wait for consumers.
     */
    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(
            consumers[i],
            NULL
        );
    }

    clock_gettime(
        CLOCK_MONOTONIC,
        &end
    );

    double elapsed =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) /
        1000000000.0;

    long long total_items =
        NUM_PRODUCERS * ITEMS_PER_PRODUCER;

    double throughput =
        total_items / elapsed;

    printf("\n========================================\n");
    printf(" Producer-Consumer Results\n");
    printf("========================================\n");

    printf("Buffer size       : %d\n", buffer_size);
    printf("Producers         : %d\n", NUM_PRODUCERS);
    printf("Consumers         : %d\n", NUM_CONSUMERS);

    printf("Expected items    : %lld\n",
           total_items);

    printf("Produced items    : %lld\n",
           produced_count);

    printf("Consumed items    : %lld\n",
           consumed_count);

    printf("Execution time    : %.6f seconds\n",
           elapsed);

    printf("Throughput        : %.2f items/sec\n",
           throughput);

    if (produced_count == total_items &&
        consumed_count == total_items)
    {
        printf("Synchronization   : CORRECT\n");
    }
    else
    {
        printf("Synchronization   : ERROR\n");
    }

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);

    free(buffer);

    return 0;
}
