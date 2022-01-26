#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "lab.h"

#define BUFFSIZE 8192

char buf[BUFFSIZE];
void writeResult(int fd);
bool eFlag = 0;
bool nFlag = 0;

int line_no = 0;    //line number variable to count the number of lines in file
int main(int argc, char *argv[])
{
    int fd;
    struct stat status;
    int option;

    while((option=getopt(argc,argv,"En"))!=-1)
        switch(option)
        {
            case 'E':
                eFlag=1;
                break;
	    case 'n':
		nFlag=1;
		break;
            case '?':
                err_msg("try 'cat --help' for more information.");
                exit(0);
        }

    if (argc-optind==0)
    {
        writeResult(STDIN_FILENO);
        exit(0);
    }
    else
        for (int i =optind; i < argc; i++)
        {
            if (strcmp(argv[i], "-") == 0)
            {
                //will check both the flags and will send the flag which is true to write function.
                writeResult(STDIN_FILENO);
            }
            else
            {
                if (lstat(argv[i],&status) < 0)
                {
                    status.st_mode=0;
                }

                if(access(argv[i],R_OK)<0 && access(argv[i],F_OK)==0)
                    err_msg("%s: Permission denied",argv[i]);

                else if(S_ISDIR(status.st_mode))
                    err_msg("%s: Is a directory",argv[i]);

                else if((fd=open(argv[i],O_RDONLY))==-1)
                    err_msg("%s: No such file or directory",argv[i]);

                else
                {
                    //will check both the flags and will send the flag which is true to write function.
                    writeResult(fd);
                    close(fd);
                }
            }
        }

    exit(0);
}

void writeResult(int fd)
{
    int n = 0;
    int newline = 1;
    bool Flag = eFlag | nFlag;
	
    if(!Flag)
    {
    while ((n = read(fd, buf, BUFFSIZE)) > 0)
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("standard output");
    }
    else

    {
        char ck[255];
        while(read(fd,buf,1) > 0)
        {
            // if nFlag is set then keep track of line_no.
            // newline variable for checking the start of a line.
            if (nFlag && newline)
            {
                line_no++;
                //converting line number to string to add in output.
                sprintf(ck,"%6d\t",line_no);
                int x = strlen(ck);
                //writing the Line Number along with the file.
                if(write(STDOUT_FILENO,ck,x)!=x)
                    err_sys("standard output");
                newline = 0;
            }
            // newline = 1 determines that the newline has started 
            // and if eFlag is set then display $ at the end of line.
            if (buf[0] == '\n' && eFlag)
            {
                if(write(STDOUT_FILENO,"$\n",2)!=2)
                {	
                    err_sys("standard output");
                }
                newline = 1;	
            }
            else
            {
                if(write(STDOUT_FILENO,buf,1)!=1)
                    err_sys("standard output");
                if(buf[0] =='\n')
                    newline = 1;
            }
        }
    }

    if(n<0)
        err_sys("read error");
}
