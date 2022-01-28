/* Exercitiu:
    Propun realizarea unui program ce are ca scop implementarea algoritmului de planificare First Come First Served (FCFS). 
    Numarul total de procese va fi citit direct din standard input, urmand ca pentru fiecare proces sa se specifice timpul sosirii si timpul de executie.
    Se va calcula timpul mediu de raspuns, timpul mediu de asteptare si se va afisa ordinea de executie a proceselor.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include "lab.h"

void assignValue(int*, int**, int**, int**);
void averages(int*, int**, int**, int**);
void completionTime(int*, int**, int**, int**);
void swap(int*, int*, int*);
void sort(int*, int**, int**, int**);
void fcfs(int*, int**, int**, int**, int**);

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
    int* arrival = (int*)malloc(numProcesses * sizeof(int));
    int* burst = (int*)malloc(numProcesses * sizeof(int));
    int* completion = (int*)malloc(numProcesses * sizeof(int));

    assignValue(&numProcesses, &arrival, &burst, &processesNum);
    fcfs(&numProcesses, &arrival, &burst, &processesNum, &completion);

    free(processesNum);
    free(arrival);
    free(burst);
    free(completion);

    return 0;
}

void fcfs(int *numProcesses, int**arrival, int **burst, int **processesNum, int **completion)
{
    sort(numProcesses, &(*arrival), &(*burst), &(*processesNum));
    completionTime(numProcesses, &(*arrival), &(*burst), &(*completion));
    averages(numProcesses, &(*arrival), &(*burst), &(*completion));    
}

void averages(int *numProcesses, int **arrival, int **burst, int **completion)
{
    double averageTurn = 0;
    double averageWait = 0;

    for(int i=0; i < *numProcesses; i++)
    {
        if(i == 0 && *(*arrival + i) != 0)
        {
            for(int i = 0; i < *numProcesses; i++)
                *(*arrival + i) = 0;
        }

        averageTurn += *(*completion + i) - *(*arrival + i);
        averageWait += ( *(*completion + i) - *(*arrival + i)) - *(*burst + i);
    }
    printf("\nAverage TurnAroundTime: %.2f", averageTurn / *numProcesses);
    printf("\nAverage WaitingTime: %.2f", averageWait / *numProcesses);
}
void completionTime(int *numProcesses, int **arrival, int **burst, int **completion)
{   
    int temp = *(*burst + 0);
    for(int i=1; i <= *numProcesses; i++)
    {
        *(*completion + (i-1)) = temp;
        if(temp < *(*arrival + i)) 
        {
            temp = *(*arrival + i) + *(*burst + i);
            continue;
        }
        else if(temp > *(*arrival + i))
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
void sort(int *numProcesses, int **arrival, int **burst, int **processesNum)
{
    int temp;
    for(int i=0;i < *numProcesses; i++)
    {
        for(int j=0;j <= i; j++)
        {
            if(*(*arrival + i) < *(*arrival + j))
            {
                swap((*arrival + i), (*arrival + j), &temp);
                swap((*burst + i), (*burst + j), &temp);
                swap((*processesNum + i), (*processesNum + j), &temp);
            }
        }
    }
    for(int i=0; i <= *numProcesses; i++)
    {
        for(int j=i+1; j <= *numProcesses - 1; j++)
        {
            if(*(*arrival + i) == *(*arrival + j))
            {
                printf("arrival[%d]=%d, arrival[%d]=%d \n", i, *(*arrival + i), j, *(*arrival + j));

                if(*(*processesNum + i) > *(*processesNum + j))
                {
                    swap((*arrival + i), (*arrival + j), &temp);
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

    printf("\nArrivals:\n");
    for(int i=0; i < *numProcesses; i++)
        printf("%d ", *(*arrival + i));

    printf("\nProcesses:\n");
    for(int i=0; i < *numProcesses; i++)
        printf("%d ", *(*processesNum + i));
}

void assignValue(int *numProcesses, int **arrival, int **burst, int **processesNum)
{
    for(int i=0; i < *numProcesses; i++)
    {
        *(*processesNum + i) = i+1;
        do
        {
            printf("Arrival time for P[%d]: ", i+1);
            if(scanf("%d", &(*(*arrival + i))) != 1)
            {
                err_msg("Failed to read integer!\n");
                exit(1);
            }
        } while (*(*arrival + i) < 0);

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

    printf("\nArrival times for processes:\n");
    for(int i=0; i < *numProcesses; i++)
        printf("%d ", *(*arrival + i));

    printf("\nBurst times for processes:\n");
    for(int i=0; i< *numProcesses; i++)
        printf("%d ", *(*burst + i));

    printf("\n");
}
