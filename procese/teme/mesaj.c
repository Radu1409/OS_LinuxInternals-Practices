#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *valoare = getenv("MESAJ"); 
    printf("Valoare variabila MESAJ : %s\n",valoare);

    return 0;
}

