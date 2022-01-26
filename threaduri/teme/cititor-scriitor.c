#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_NUMBERS 10
#define NO_OF_WRITERS 5
#define NO_OF_READERS 10

int numbers[MAX_NUMBERS];
pthread_mutex_t mutex;
sem_t writer;
struct timespec delay;
int idx = 0;
int readerCount = 0;

void produce_item(int wno)
{
    puts("- producing -");
    int rand_number = rand() % 1000;
    numbers[idx++] = rand_number;
    printf("Writer %d wrote %d in array at index %d\n", wno, rand_number, idx - 1);
    sleep(1);
}

void consume_item(int *position, int rno)
{
    puts("- consuming -");

    for (int i = *position; i < idx; i++)
    {
        printf("Reader %d: read %d from array from index %d\n", rno, numbers[i], i);
    }
    *position = idx - 1;
}

void writer_function(void *wno)
{
    while (1)
    {

        sem_wait(&writer);
        if (idx >= MAX_NUMBERS)
        {
            sem_post(&writer);
            break;
        }
        produce_item((*((int *)wno)));
        sem_post(&writer);
        nanosleep(&delay, NULL);
    }
}

void reader_function(void *rno)
{
    int position = 0;
    while (1)
    {
        // Cititorul blocheaza lacatul inainte sa modifice readerCount
        pthread_mutex_lock(&mutex);
        readerCount++;
        if (readerCount == 1)
        {
            sem_wait(&writer); // Daca ii primul cititor, acesta va bloca scriitorii
        }
        pthread_mutex_unlock(&mutex);

        consume_item(&position, (*((int *)rno)));

        // Cititorul blocheaza lacatul inainte sa modifice readerCount
        pthread_mutex_lock(&mutex);
        readerCount--;
        if (readerCount == 0)
        {
            sem_post(&writer); // Daca ii ultimul cititor, acesta va trezi unul dintre scriitorul
        }
        pthread_mutex_unlock(&mutex);
        nanosleep(&delay, NULL);

        if (position >= MAX_NUMBERS - 1)
        {
            break;
        }
    }
}

int main()
{

    delay.tv_sec = 2;
    delay.tv_nsec = 0;

    pthread_t readers[NO_OF_READERS], writers[NO_OF_WRITERS];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&writer, 0, 1);

    int a[50]; //index pentru cititori si scriitori

    for (int i = 0; i < NO_OF_WRITERS; i++)
    {
        a[i] = i + 1;
        pthread_create(&writers[i], NULL, (void *)writer_function, (void *)&a[i]);
    }

    for (int i = 0; i < NO_OF_READERS; i++)
    {
        a[i] = i + 1;
        pthread_create(&readers[i], NULL, (void *)reader_function, (void *)&a[i]);
    }

    for (int i = 0; i < NO_OF_READERS; i++)
    {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NO_OF_WRITERS; i++)
    {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&writer);
}