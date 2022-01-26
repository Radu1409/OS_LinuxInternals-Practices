#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "lab.h"

void generateRandomNumbers(int n, int *v)
{
    for (int i = 0; i < n; i++)
    {
        v[i] = rand() % 1000; //numbers 0->999
    }
}

int main(int argc, char **argv)
{
    int n; //size of vector
    int fd[2];
    if (pipe(fd) < 0)
        err_sys("Eroare creare pipe");

    if (argc != 2)
        err_sys("Usage ./sort <vector_size>");

    int pid = fork();
    if (pid == 0) //child process
    {
        close(fd[1]);
        dup2(fd[0], 0);
        execlp("sort", "sort", "-n", NULL);
    }

    close(fd[0]);

    n = atoi(argv[1]);
    int *numbers = malloc(n * sizeof(int));
    generateRandomNumbers(n, numbers);

    for (int i = 0; i < n; i++)
    {
        char str[10];
        sprintf(str, "%d\n", numbers[i]);
        if (write(fd[1], str, strlen(str)) < 0)
            err_sys("Eroare la scriere");
    }
    close(fd[1]);
    wait(NULL);
    exit(0);
}
