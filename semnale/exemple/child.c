#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "lab.h"

static void sig_cld();

int main()
{
    pid_t   pid;

    if (signal(SIGCHLD, sig_cld) == SIG_ERR)
        perror("signal error");

    if ( (pid = fork()) < 0)
        perror("fork error");
    else if (pid == 0) {        /* child */
        sleep(2);
        _exit(0);
    }
    pause();    /* parent */
    exit(0);
}

static void sig_cld()
{
    pid_t   pid;
    int     status;

    printf("SIGCHLD received\n");
    if (signal(SIGCHLD, sig_cld) == SIG_ERR)    /* reestablish handler */
        err_msg("signal error");

    if ( (pid = wait(&status)) < 0)     /* fetch child status */
        err_msg("wait error");
    printf("pid = %d\n", pid);
    return;     /* interrupts pause() */
}
