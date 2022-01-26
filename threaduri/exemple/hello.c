#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define err_msg(msg) \
    do { \
        perror(msg); \
        exit(1); \
    } while (0)

void print_message_function(void *ptr)
{
    char *message;
    message = (char *)ptr;
    printf("%s ", message);
}

main()
{
    pthread_t thread1, thread2;
    char *message1 = "Hello";
    char *message2 = "world";
    pthread_attr_t attr;
    int e;
    
    e = pthread_create(&thread1, NULL,
            (void *)&print_message_function, (void *)message1);
    if (e)
    {
        errno = e;
        err_msg("Creating thread1");
    }
    
    e = pthread_create(&thread2, NULL,
            (void *)&print_message_function, (void *)message2);
    if (e)
    {
        errno = e;
        err_msg("Creating thread2");
    }
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    exit(0);
}
