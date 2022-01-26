#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "lab.h"
#define buf_size 8000

char buf[100];
char buf2[buf_size];
int main(int argc, char *argv[])
{
    int fd;
    int fd2;

    if (argc > 2 || argc < 2)
    {
        printf("Usage : %s <input file>\n", argv[0]);
        exit(0);
    }

    // deschidere fisierul de intrare primit ca parametru
    if ((fd = open(argv[1], O_RDWR)) == -1)
        err_sys("open error");

    // creeare fisier de iesire -fara gauri-
    if ((fd2 = creat("output.file", FILE_MODE)) == -1)
        err_sys("creat error");

    size_t j = 0;

    // citire caractere din fisier
    while (read(fd, buf, 1) > 0)
    {

        if (buf[0] != '\0')
        {
            buf2[j] = buf[0];
            j++;
        }

        if (j == buf_size)
        {

            j = 0;
            if (write(fd2, buf2, strlen(buf2)) == -1)
                err_sys("write error 1");

            for (size_t k = 0; k < buf_size; k++)
                buf2[k] = '\0';
        }
    }

    //scriere continut fisier de intrare in fisier iesire fara octetii de 0
    if (write(fd2, buf2, strlen(buf2)) == -1)
        err_sys("write error 2");

    close(fd);
    close(fd2);

    exit(0);
}