/* Exercitiu
        Să se implementeze apelul execvp() cu ajutorul lui execv().
*/

#include "lab.h"
#include <errno.h>
#include <stdio.h>

#define NAME_DIM 1000

int execvp1(char*, char**);

int main(int argc, char* argv[])
{
    //char *args[] = {"ls","-l", NULL};
    if (argc < 2)
    {
        err_msg("error: too few arguments to '%s'", __FILE__);
        exit(1);
    }

    execvp1(argv[1], &argv[1]);
    //execv("/bin/ls", args);
    //execvp("ls", args);

    printf("This line will not be executed!\n");

    return 0; 
}

int execvp1(char* name, char** argv)
{
    char fullName[NAME_DIM+1];
    char *pathName; 

    register char *first, *last;    /* "register" indica compilatorului sa stocheze variabila 

				       in registru CPU in loc de memorie. Variabilele utilizate 

				       frecvent sunt pastrate in registre si au o accesibilitate 

				       mai rapida */

    int nameLength, size, noAccess;
    noAccess = 0;

    if (index(name, '/') != 0) 
    {
        execv(name, argv);
        return -1;
    }

    pathName = getenv("PATH");

    if (pathName == 0) 
    {
        pathName = "/sprite/cmds";
    }

    nameLength = strlen(name);

    for (first = pathName; ; first = last+1) 
    {
        for (last = first; (*last != 0) && (*last != ':'); last++) {}

        size = last-first;

        if ((size + nameLength + 2) >= NAME_DIM) 
        {
            continue;
        }
        strncpy(fullName, first, size);

        if (last[-1] != '/') 
        {
            fullName[size] = '/';
            size++;
        }
        
        strcpy(fullName + size, name);
        execv(fullName, argv);

        if (errno == EACCES) 
        {
            noAccess = 1;
        } 
        
        else if (errno != ENOENT) 
        {
            break;
        }
        
        if (*last == 0) 
        {    
            /*
            Daca a existat un fisier cu numele potrivit de-a lungul path-ului,
            dar permisiunile sale au fost gresite, se returneaza FS_NO_ACCES.
            */

            if (noAccess) 
            {
                errno = EACCES;
            }
            break;
        }
    }
    return -1;
}
