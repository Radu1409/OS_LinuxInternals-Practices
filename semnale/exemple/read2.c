#include    <setjmp.h>
#include    <signal.h>
#include    "lab.h"

static void     sig_alrm(int);
static jmp_buf  env_alrm;

int main()
{
    int     n;
    char    line[MAXLINE];

    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        err_sys("signal(SIGALRM) error");

    if (setjmp(env_alrm) != 0)
        err_quit("read timeout");

    alarm(10);
    if ( (n = read(STDIN_FILENO, line, MAXLINE)) < 0)
        err_sys("read error");
    alarm(0);

    if (write(STDOUT_FILENO, line, n) != n)
        err_sys("write_error");

    exit(0);
}

static void sig_alrm(int signo)
{
    (void)signo;
    longjmp(env_alrm, 1);
}
