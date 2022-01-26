#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include "lab.h"

int main(int argc, char **argv)
{
    int option, k;
    bool wflag = 0;
    bool iflag = 0;

    while ((option = getopt(argc, argv, "wi")) != -1)
    {
        switch (option)
        {
            case 'w':
                wflag = 1;
                break;
            case 'i':
                iflag = 1;
                break;
            case '?':
                fprintf(stderr,
                        "Usage: grep [OPTION]... PATTERNS [FILE]...\nTry 'grep "
                        "--help' for more information.\n");
                exit(0);
                break;
        }
    }

    struct stat statbuf;

    if (argc - optind == 0)
    {
        fprintf(stderr,
                "Usage: grep [OPTION]... PATTERNS [FILE]...\nTry 'grep "
                "--help' for more information.\n");
        exit(2);
    } 
    else if (argc - optind < 2)
    {
        // when file not provided, it will read stdin
        size_t len = 0;
        ssize_t read = 0;
        k = 0;

        while (1)
        {
            char *line = NULL;
            // reading stdin line by line
            read = getline(&line, &len, stdin);
            if (read == -1) break;

            // replace new line character with null character at the end of line
            line[strcspn(line, "\n")] = '\0';
            char *ret = line;

            // for loop to check multiple occurrences in line
            for (;;)
            {
                ret = strstr(ret, argv[optind]);
                if (ret == NULL) break;

                if (!wflag)
                {
                    printf("%s\n", line);
                    break;
                }
                else 
                {
                    // if wflag is set, check if there is alphanumeric charcter at beginning
                    if ((ret == line) || !isalnum((unsigned char)ret[-1]))
                    {
                        ret += strlen(argv[2]);
                        // to check no character at the end
                        if (!isalnum((unsigned char)*ret))
                        {
                            printf("%s\n", line);
                            break;
                        }
                    }
                    ret += 1;
                }
            }
            free(line);
        }
    } 
    else 
    {
        for (int i = optind + 1; i < argc; i++)
        {
            if (lstat(argv[i], &statbuf) == 0)
            {
                if (S_ISDIR(statbuf.st_mode))
                {
                    err_msg("%s: Is a directory", argv[i]);
                    return 2;
                }
            }

            int fd = open(argv[i], O_RDONLY);
            if (fd < 0)
            {
                err_ret("%s", argv[i]);
                return 2;
            }

            int size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            char c;
            k = 0;

            for (int j = 0; j < size; j++)
            {
                int r = read(fd, &c, 1);
                if (r == -1) err_sys("read error\n");
                // increment k to count the characters before newline character
                k++;
                if (c == '\n')
                {
                    bool found = false;
                    char *sir = malloc((k + 1) * sizeof(char));
                    lseek(fd, -k, SEEK_CUR);
                    int r1 = read(fd, sir, k - 1);

                    if (r1 == -1) err_sys("read error\n");

                    sir[k - 1] = '\0';
                    char ck[1024] = {0};

                    char *ret = sir;
                    // for loop to check multiple occurences in line
                    for (;;)
                    {
                        if(iflag == 0)
                            ret = strstr(ret, argv[optind]);
                        else
                            ret = strcasestr(ret,argv[optind]);

                        if (ret == NULL) break;
                        
                        // if multiple files are present, print filename at the beginning
                        if(argc-optind > 2)
                            sprintf(ck, "%s:", argv[i]);

                        if (!wflag)
                        {
                            strcat(ck, sir);
                            found = true;
                            break;
                        } 
                        else 
                        {
                            if ((ret == sir) || !isalnum((unsigned char)ret[-1]))
                            {
                                ret += strlen(argv[optind]);
                                if (!isalnum((unsigned char)*ret))
                                {
                                    strcat(ck, sir);
                                    found = true;
                                    break;
                                }
                            }
                            ret += 1;
                        }
                    }
                    // print buffer if not empty
                    if (found)
                        printf("%s\n", ck);
                    //make buffer empty
                    memset(ck, 0, 1024*sizeof(ck[0]));
                    
                    free(sir);
                    lseek(fd, j + 1, SEEK_SET);
                    k = 0;
                }
            }
            close(fd);
        }
        exit(0);
    }
    return 0;
}

