#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // numarul de filozofi

// filozofii sunt asezati la o masa circulara, LEFT si RIGHT indecsii vecinilor filozofului philosopher_number
#define LEFT(philosopher_number) ((philosopher_number + N - 1) % N)
#define RIGHT(philosopher_number) ((philosopher_number + 1) % N)

//Ce poate face un filozf
typedef enum
{
    THINKING,
    HUNGRY,
    EATING
} state_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
state_t state_array[N]; // vectorul care tine evidenta starii fiecarui filozof
sem_t forks[N];         // coordonatele furculitelor

// Filozoful gandeste
void think(int philosopher_number)
{
    printf("Philosopher %i is thinking...\n", philosopher_number);
    sleep(2);
}

// FIlozoful mananca ...
void eat(int philosopher_number)
{
    printf("Philosopher %i is eating...\n", philosopher_number);
    sleep(1);
}

// testez sa vad daca un filozof poate manca
void test(int philosopher_number)
{
    if (state_array[philosopher_number] == HUNGRY && state_array[LEFT(philosopher_number)] != EATING && state_array[RIGHT(philosopher_number)] != EATING)
    {
        state_array[philosopher_number] = EATING;
        printf("Philosopher %d trying to take fork %d and %d...\n", philosopher_number, LEFT(philosopher_number), philosopher_number);
        sem_post(&forks[philosopher_number]);
    }
}

// filozoful incearca sa ia furculitele
void take_forks(int philosopher_number)
{
    pthread_mutex_lock(&mutex);
    state_array[philosopher_number] = HUNGRY;
    printf("Philosopher %d is hungry...\n", philosopher_number);
    test(philosopher_number); //testez sa vad daca poate manca
    pthread_mutex_unlock(&mutex);
    sem_wait(&forks[philosopher_number]); //daca reuseste sa obtina permisiunea dupa test acesta mananca, altfel asteapta
}

// filozoful elibereaza furculitele
void put_forks(int philosopher_number)
{
    pthread_mutex_lock(&mutex);
    state_array[philosopher_number] = THINKING;
    printf("Philosopher %d putting fork %d and %d down...\n", philosopher_number, LEFT(philosopher_number), philosopher_number); //pune jos furculitele
    test(LEFT(philosopher_number));                                                                                              // semnaleaza vecinii ca pot incerca sa manance
    test(RIGHT(philosopher_number));
    pthread_mutex_unlock(&mutex);
}

//Functia executata de thread-uro
void *philosopher(void *arg)
{
    // index-ul unui filozof (pozitia la masa)
    int philosopher_number = *(int *)arg;
    free(arg);

    while (1)
    {
        think(philosopher_number);
        take_forks(philosopher_number);
        eat(philosopher_number);
        put_forks(philosopher_number);
    }

    return NULL;
}

int main(void)
{
    int philosopher_number;
    int i;
    int *arg;
    pthread_t threadIDs[N];

    // Iitializarea semafoarelor
    for (i = 0; i < N; ++i)
    {
        sem_init(&forks[i], 0, 0);
    }

    // Creem filozofii si le asignam o pozitie la masa
    for (philosopher_number = 0; philosopher_number < N; ++philosopher_number)
    {
        arg = (int *)malloc(sizeof(int));
        *arg = philosopher_number;
        pthread_create(&threadIDs[philosopher_number], NULL, philosopher, arg);
    }

    for (philosopher_number = 0; philosopher_number < N; ++philosopher_number)
    {
        pthread_join(threadIDs[philosopher_number], NULL);
    }

    for (i = 0; i < N; ++i)
    {
        sem_destroy(&forks[i]);
    }
    return EXIT_SUCCESS;
}
