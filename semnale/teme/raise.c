#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void signal_handler(int signal)
{
    printf("Received signal %d\n", signal);
}

int main(void)
{

    signal(SIGTERM, signal_handler);

    printf("Sending signal %d\n", SIGTERM);
    kill(getpid(), SIGTERM); //functia raise() trimite un semnal catre proces
    printf("Exit main()\n");
}
