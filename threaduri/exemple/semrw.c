#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

char buffer[100];
sem_t writers_turn;
sem_t readers_turn;

void produce_item(void)
{
    puts("- producing -");
    scanf("%s", buffer);
}

void consume_item(void)
{
    puts("- consuming -");
    printf("%s\n", buffer);
}

void writer_function(void)
{
    while (1)
    {
        sem_wait(&writers_turn);
        produce_item();
        sem_post(&readers_turn);
    }
}

void reader_function(void)
{
    while (1)
    {
        sem_wait(&readers_turn);
        consume_item();
        sem_post(&writers_turn);
    }
}

main()
{
    pthread_t reader;

    sem_init(&writers_turn, 0, 1);
    sem_init(&readers_turn, 0, 1);

    /* writer must go first */
    sem_wait(&readers_turn);

    pthread_create(&reader, NULL, (void *)&reader_function, NULL);

    writer_function();
}