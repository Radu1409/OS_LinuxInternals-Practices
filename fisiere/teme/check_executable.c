#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int check_permissions(int permission)
{
    int mask=0111;
    int res = permission & mask;
    if(res>0)
        return 1;
    return 0;
}

void check_file(char *path)
{
    struct stat statbuf;

    if(lstat(path, &statbuf) == 0) {
        if(S_ISREG(statbuf.st_mode)){
            if(check_permissions(statbuf.st_mode)==1) {

                printf("Fisierul este executabil. ");
                int fd=open(path,O_RDONLY);
                char buf[11];
                read(fd,buf,10);

                if(strncmp(buf,"#!/bin/bash",10)==0 || strncmp(buf,"#!/bin/sh",8)==0)
                    printf("Este script shell.\n");
                else
                    printf("Este fisier binar.\n");
                close(fd);
            }
            else
                printf("Fisierul nu este executabil.\n");
        }
        else
            printf("Nu este fisier.\n");
    }
    else
        printf("Fisierul nu exista.\n");
    return;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Numar gresit de parametrii.\n");
        return -1;
    }

    check_file(argv[1]);
    return 0;
}       

