#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "lab.h"

int main(int argc, char* argv[])
{
    struct stat file;

    if(argc == 1)
    {
        err_msg("missing file operand\nTry 'rm --help' for more information.");
        exit(1);
    }

    for(int index = 1; index < argc; index++)
        if(stat(argv[index],&file) == 0)
        {  
            if(S_ISREG(file.st_mode) != 0)
            {
                if(remove(argv[index]) == -1)
                    err_ret("cannot remove '%s'",argv[index]);
            }
            else
                err_msg("cannot remove '%s': Is a directory",argv[index]);
        }
        else
            err_ret("cannot remove '%s'",argv[index]);
    
    exit(0);
}

