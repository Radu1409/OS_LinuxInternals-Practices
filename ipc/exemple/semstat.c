/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 *****************************************************************************
 MODULE: semstat.c
 *****************************************************************************
 A companion command line tool for the semtool package.  semstat displays
 the current value of all semaphores in the set created by semtool.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
    /* union semun is defined by including <sys/sem.h> */
#else
    /* according to X/OPEN we have to define it ourselves */
    union semun {
        int val;                /* value for SETVAL */
        struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;  /* array for GETALL, SETALL */
        struct seminfo *__buf;      /* buffer for IPC_INFO */
    };
#endif

int get_sem_count(int sid);
void show_sem_usage(int sid);
int get_sem_count(int sid);
void dispval(int sid);

int main()
{
    key_t key;
    int   semset_id;

    /* Create unique key via call to ftok() */
    key = ftok(".", 's');

    /* Open the semaphore set - do not create! */
    if((semset_id = semget(key, 1, 0666)) == -1)
    {
        printf("Semaphore set does not exist\n");
        exit(1);
    }

    show_sem_usage(semset_id);
    return(0);
}

void show_sem_usage(int sid)
{
    int cntr=0, maxsems, semval;

    maxsems = get_sem_count(sid);

    while(cntr < maxsems)
    {
        semval = semctl(sid, cntr, GETVAL, 0);
        printf("Semaphore #%d:  --> %d\n", cntr, semval);
        cntr++;
    }
}

int get_sem_count(int sid)
{
    int rc;
    struct semid_ds mysemds;
    union semun semopts;

    /* Get current values for internal data structure */
    semopts.buf = &mysemds;

    if((rc = semctl(sid, 0, IPC_STAT, semopts)) == -1) {
        perror("semctl");
        exit(1);
    }

    /* return number of semaphores in set */
    return(semopts.buf->sem_nsems);
}

void dispval(int sid)
{
    int semval;

    semval = semctl(sid, 0, GETVAL, 0);
    printf("semval is %d\n", semval);
}

