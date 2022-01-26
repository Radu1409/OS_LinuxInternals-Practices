#include "lab.h"

int main(void)
{
    int    n, fd[2];
    pid_t  pid;
    char   line[MAXLINE];

    if (pipe(fd) < 0)
        err_sys("pipe error");

    if ( (pid = fork()) < 0)
        err_sys("fork error");

    else if (pid > 0) {     /* parent */
        close(fd[0]);
        if (write(fd[1], "hello world\n", 12) != 12)
            err_sys("write error");

    } else {                /* child */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        if (write(STDOUT_FILENO, line, n) != n)
            err_sys("write error");
    }

    exit(0);
}
