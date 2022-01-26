#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "lab.h"

int main()
{
    pid_t child_pid;
    child_pid = fork ();

    if (child_pid > 0)
    {
        int sys = system("ps");

        if( sys == -1)
            err_sys("system error\n");
        else
            sleep (5);
    }
    else
    {
        exit(0);
    }
    return 0;
}
