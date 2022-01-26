#include <pthread.h>
#include <stdio.h>

char buffer[100];
int buffer_has_item = 0;
pthread_mutex_t mutex;
struct timespec delay;

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
        pthread_mutex_lock(&mutex);
        if (buffer_has_item == 0)
        {
            produce_item();
            buffer_has_item = 1;
        }
        pthread_mutex_unlock(&mutex);
        nanosleep(&delay, NULL);
    }
}

void reader_function(void)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (buffer_has_item == 1)
        {
            consume_item();
            buffer_has_item = 0;
        }
        pthread_mutex_unlock(&mutex);
        nanosleep(&delay, NULL);
    }
}

main()
{
    pthread_t reader;

    delay.tv_sec = 2;
    delay.tv_nsec = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&reader, NULL,(void *)&reader_function, NULL);

    writer_function();
}