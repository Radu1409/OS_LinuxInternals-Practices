/* Exercitiu: 
    Realizati un program ce are ca scop implementarea algoritmului de planificare Shortest Job First (SJF). 
    Numarul total de procese va fi citit direct din standard input, urmand ca pentru fiecare proces sa se specifice timpul de executie.
    Se va calcula timpul mediu de asteptare si se va afisa ordinea de executie a proceselor.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include "lab.h"

void assignValue(int*, int**, int**);
void averages(int*, int**, int**);
void completionTime(int*, int**, int**);
void swap(int*, int*, int*);
void sort(int*, int**, int**);
void shjb(int*, int**, int**, int**);

int main(void)
{
    int numProcesses;
    do
    {
        printf("Total number of processes: ");
        if(scanf("%d", &numProcesses) != 1)
        {
            err_msg("Failed to read integer!");
            exit(1);
        }
    } while (numProcesses < 0);

    int* processesNum = (int*)malloc(numProcesses * sizeof(int));
    int* burst = (int*)malloc(numProcesses * sizeof(int));
    int* completion = (int*)malloc(numProcesses * sizeof(int));

    assignValue(&numProcesses, &burst, &processesNum);
    shjb(&numProcesses, &burst, &processesNum, &completion);

    free(processesNum);
    free(burst);
    free(completion);

    return 0;
}

void shjb(int *numProcesses, int **burst, int **processesNum, int **completion)
{
    sort(numProcesses, &(*burst), &(*processesNum));
    completionTime(numProcesses, &(*burst), &(*completion));
    averages(numProcesses, &(*burst), &(*completion));    
}

void averages(int *numProcesses, int **burst, int **completion)
{
    double averageTurn = 0;
    double averageWait = 0;

    for(int i=0; i < *numProcesses; i++)
    {
        averageTurn += *(*completion + i);
        averageWait += *(*completion + i) - *(*burst + i);
    }
    printf("\nAverage TurnAroundTime: %.2f", averageTurn / *numProcesses);
    printf("\nAverage WaitingTime: %.2f", averageWait / *numProcesses);
}
void completionTime(int *numProcesses, int **burst, int **completion)
{   
    int temp = *(*burst + 0);
    for(int i=1; i <= *numProcesses; i++)
    {
        *(*completion + (i-1)) = temp;
        temp = temp + *(*burst + i);
    }
    printf("\nCompletion Time: \n");
    for(int i=0; i < *numProcesses; i++)
        printf("%d ", *(*completion + i));
}
void swap(int *v1, int *v2, int *temp)
{
    *temp = *v1;
    *v1 = *v2;
    *v2 = *temp;
}
void sort(int *numProcesses, int **burst, int **processesNum)
{
    int temp;
    for(int i=0;i < *numProcesses; i++)
    {
        for(int j=0;j <= i; j++)
        {
            if(*(*burst + i) < *(*burst + j))
            {
                swap((*burst + i), (*burst + j), &temp);
                swap((*processesNum + i), (*processesNum + j), &temp);
            }
        }
    }
    for(int i=0; i <= *numProcesses; i++)
    {
        for(int j=i+1; j <= *numProcesses - 1; j++)
        {
            if(*(*burst + i) == *(*burst + j))
            {
                if(*(*processesNum + i) > *(*processesNum + j))
                {
                    swap((*burst + i), (*burst + j), &temp);
                    swap((*processesNum + i), (*processesNum + j), &temp);
                }
            }                
        }
    }
    printf("\n\nAfter sorting in ascending order");

    printf("\nBursts:\n");
    for(int i=0; i < *numProcesses; i++)
        printf("%d ", *(*burst + i));

    printf("\nProcesses:\n");
    for(int i=0; i < *numProcesses; i++)
        printf("%d ", *(*processesNum + i));
}

void assignValue(int *numProcesses, int **burst, int **processesNum)
{
    for(int i=0; i < *numProcesses; i++)
    {
        *(*processesNum + i) = i+1;
        do
        {
            printf("Burst time for P[%d]: ", i+1);
            if(scanf("%d", &(*(*burst + i))) != 1)
            {
                err_msg("Failed to read integer!\n");
                exit(1);
            }
        } while (*(*burst + i) < 0);
    }
    printf("\nProcesses ID are: \n");
    for(int i=0; i < *numProcesses; i++)
        printf("%d ", *(*processesNum + i));

    printf("\nBurst times for processes:\n");
    for(int i=0; i< *numProcesses; i++)
        printf("%d ", *(*burst + i));
    printf("\n");
}
