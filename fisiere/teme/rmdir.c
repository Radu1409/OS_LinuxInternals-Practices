#include <unistd.h>
#include "lab.h"

int main(int argc, char **argv)
{
    if(argc == 1)
        err_msg("missing operand\nTry 'rmdir --help' for more information.");

    for(int i=1;i<argc;i++)
        if(rmdir(argv[i]) == -1)
                err_ret("failed to remove '%s'",argv[i]); 
    exit(0);
}
