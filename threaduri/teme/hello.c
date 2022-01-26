#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

#define err_msg(msg) \
    do { \
        perror(msg); \
        exit(1); \
    } while (0)

#define MSG_NO 3 

typedef struct {
    int index;
    char *msg;
} message;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waiting=PTHREAD_COND_INITIALIZER;

int current_index=0;

void print_message_function(void *ptr)
{
    message *m;
    m = (message *)ptr;
    
    pthread_mutex_lock(&mutex);
    while(m->index!=current_index+1)
    {
        pthread_cond_wait(&waiting,&mutex);
    }
    printf("%s ", m->msg);
    current_index++;
    pthread_cond_broadcast(&waiting);
    pthread_mutex_unlock(&mutex);
   
}

int main()
{
    pthread_t th[MSG_NO];
    message m[MSG_NO];
    int e;

    char *message1 = "Hello";
    char *message2 = "world";
    char *message3 = "!";

    m[0].index=1;
    m[0].msg=message1;

    m[1].index=2;
    m[1].msg=message2;

    m[2].index=3;
    m[2].msg=message3;
    
     e = pthread_create(&th[2], NULL,
            (void *)&print_message_function, (void *)(&m[2]));
    if (e)
    {
        errno = e;
        err_msg("Creating thread3");
    }

      e = pthread_create(&th[1], NULL,
            (void *)&print_message_function, (void *)(&m[1]));
    if (e)
    {
        errno = e;
        err_msg("Creating thread2");
    }

    e = pthread_create(&th[0], NULL,
            (void *)&print_message_function, (void *)(&m[0]));
    if (e)
    {
        errno = e;
        err_msg("Creating thread1");
    }
    
    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);
    pthread_join(th[2], NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&waiting);
    exit(0);
}
