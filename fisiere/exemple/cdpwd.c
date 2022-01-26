#include    "lab.h"

int
main(void)
{
    char    *ptr;
    int     size;

    ptr = path_alloc(&size);    /* our own function */
    if (getcwd(ptr, size) == NULL)
        err_sys("getcwd failed");

    printf("cwd = %s\n", ptr);

    if (chdir("/usr/local/bin") < 0)
        err_sys("chdir failed");

    if (getcwd(ptr, size) == NULL)
        err_sys("getcwd failed");

    printf("cwd = %s\n", ptr);
    exit(0);
}
