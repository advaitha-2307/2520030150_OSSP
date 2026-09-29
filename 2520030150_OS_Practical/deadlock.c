#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t resource_A =
    PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t resource_B =
    PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg)
{
    printf("Thread 1: trying to lock Resource A\n");

    pthread_mutex_lock(&resource_A);

    printf("Thread 1: locked Resource A\n");

    sleep(1);

    printf("Thread 1: trying to lock Resource B\n");

    pthread_mutex_lock(&resource_B);

    printf("Thread 1: locked Resource B\n");

    printf("Thread 1: using both resources\n");

    pthread_mutex_unlock(&resource_B);
    pthread_mutex_unlock(&resource_A);

    return NULL;
}

void *thread2(void *arg)
{
    printf("Thread 2: trying to lock Resource B\n");

    pthread_mutex_lock(&resource_B);

    printf("Thread 2: locked Resource B\n");

    sleep(1);

    printf("Thread 2: trying to lock Resource A\n");

    pthread_mutex_lock(&resource_A);

    printf("Thread 2: locked Resource A\n");

    printf("Thread 2: using both resources\n");

    pthread_mutex_unlock(&resource_A);
    pthread_mutex_unlock(&resource_B);

    return NULL;
}

int main()
{
    pthread_t t1;
    pthread_t t2;

    printf("Starting deadlock demonstration...\n\n");

    pthread_create(
        &t1,
        NULL,
        thread1,
        NULL
    );

    pthread_create(
        &t2,
        NULL,
        thread2,
        NULL
    );

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nProgram completed.\n");

    return 0;
}
