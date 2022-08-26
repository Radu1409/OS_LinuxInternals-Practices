#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "lab.h"

bool vFlag = 0;

int main(int argc, char* argv[])
{
    bool err = false;
    int option;

    // check for optional arguments
    while((option = getopt(argc,argv,"v")) !=-1)
        switch(option)
        {
            case 'v':
            vFlag = 1;
            break;
        }

    if(argc == 1)
    {
        err_msg("missing operand\nTry 'mkdir --help' for more information.");
        exit(1);
    }

    for(int i=optind; i<argc; i++)
    {
        if(access(argv[i],F_OK) == 0)
        {
            err_msg("cannot create directory ‘%s’: File exists",argv[i]);
            err = true;
        }
        else
        {
            if(mkdir(argv[i], S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1)
            {
                err_ret("cannot create directory '%s'",argv[i]);
                exit(1);
            }
            else
                if (vFlag)
                    printf("mkdir: created directory '%s'\n", argv[i]);
        }
    }
    if(err == false)
        exit(0);
    else
        exit(1);
}
