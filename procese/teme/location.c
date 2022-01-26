#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <limits.h>

int main(int argc,char **argv) {
    char buf[PATH_MAX];
    (void)argc;
    char *res = realpath(argv[0],buf);
    if(res)
        printf("%s\n", buf);
    return 0;
}  

