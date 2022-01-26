/* Exercitiu:
        Implementați un mecanism prin care un proces părinte poate să determine pid-urile tuturor proceselor fiu prin apelarea unei funcții care 
        le întoarce un vector creat dinamic cu pid-urile copiilor, respectiv numărul total al elementelor din vector (adică numărul de fii)
        este întors într-un parametru (pointer la întreg)
*/

#include <sys/types.h>
#include <sys/wait.h>
#include "lab.h"

int *createProcesses(int**, int*);

int main(void)
{
    int numPids = 0;
    int* processIDS = (int*)malloc(1 * sizeof(int));

    createProcesses(&processIDS, &numPids);
    createProcesses(&processIDS, &numPids);
    createProcesses(&processIDS, &numPids);

    printf("\nThe vector with the pids of all processes: \n");

    for(int i = 0; i < numPids; i++)
    {
        if (i == numPids - 1)
        {
            printf("%d.", *(processIDS + i));
            break;
        }
        printf("%d, ", *(processIDS + i));
    }
    printf("\nThe total number of elements in the vector: %d \n", numPids);
    free(processIDS);
    
    return 0;
}

int *createProcesses(int** processIDS, int* numPids)
{
    pid_t pid;

    if(*numPids == 0)
        printf("Parent Process: PID %d\n", getpid());

    if ((pid = fork()) < 0)
        err_sys("fork error!");

    else if (pid == 0) 
    {
        while(1) {}
    }

    else if (pid > 0) 
    {
        (*numPids)++;
        *(*processIDS + (*numPids - 1)) = pid;
    }

    *processIDS = realloc(*processIDS, (*numPids + 1) * sizeof(int));

    /* 
    printf("\nThe vector with the pids of all processes: \n");
    for(int i = 0; i < nProcess; i++) 
    {
        if (i == nProcess - 1)
        {
            printf("%d. ", *(processIDS + i));
            break;
        }
        printf("%d, ", *(processIDS + i));
    }
    printf("\nThe total number of elements in the vector: %d\n", *numPids);
    */

    return *processIDS;
}
