#include    <pwd.h>
#include    <signal.h>
#include    "lab.h"

static void my_alarm(int);

int main()
{
    struct passwd   *ptr;

    signal(SIGALRM, my_alarm);
    alarm(1);

    for ( ; ; ) {
        if ( (ptr = getpwnam("student")) == NULL)
            err_sys("getpwnam error");
        if (strcmp(ptr->pw_name, "student") != 0)
            printf("return value corrupted!, pw_name = %s\n",
                    ptr->pw_name);
    }
}

static void my_alarm(int signo)
{
    (void)signo;
    struct passwd   *rootptr;

    printf("in signal handler\n");
    if ( (rootptr = getpwnam("nobody")) == NULL)
            err_sys("getpwnam(root) error");
    alarm(1);
    return;
}
