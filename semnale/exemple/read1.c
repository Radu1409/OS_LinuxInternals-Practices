#include    <signal.h>
#include    "lab.h"

static void sig_alrm(int);

int main()
{
    int     n;
    char    line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");
    alarm(10);
    if ( (n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");
    alarm(0);

    if (write(STDOUT_FILENO, line, n) != n)
        err_sys("write error");

    exit(0);
}

static void sig_alrm(int signo)
{
    (void)signo;
    printf("alarm\n");
    return; /* nothing to do, just return to interrupt the read */
}
