#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>
#include "lab.h"

#define BUFFSIZE 4096

/* function for trying to parse the argument for n option */
void tryParse(char *arg);

/* function for write output */
void writeOutput(int fd);

/* options */
bool nFlag = true;
bool cFlag = false;
bool signed_zero = false;


int file_size = 0;
int line_number = 10;
int char_number = 1;

char read_buf[2];
char write_buf[BUFFSIZE];

int main(int argc,char **argv)
{
    struct stat status;
    int fd; //file descriptor for files
    int option; // current option
    bool first_print = false;
    bool all_wrong = true;
    char *arg="", *err;

    while(( option = getopt(argc,argv,"n:c:") ) != -1)
        switch(option)
        {
            case 'n':
                nFlag = true;
                cFlag = false;
                arg = optarg;
                tryParse(arg);
                break;
            case 'c':
                cFlag = true;
                nFlag = false;
                arg = optarg;
                tryParse(arg);
                break;
            case '?':
                err = "Try 'head --help' for more information.\n";
                if(write(STDERR_FILENO,err,strlen(err)) == -1)
                    err_sys("stderr write error");
                exit(EXIT_FAILURE);
                break;
        }


    /* if no files are given , entry on stdin */
    if(argc - optind == 0)
    {
        if(((nFlag && line_number==0) || (cFlag && char_number==0)) && !signed_zero)
            exit(0);
		
        if(((nFlag && line_number==0) || (cFlag && char_number==0)) && signed_zero)
        {
            int j=0;
            char *buf = malloc(1024 * sizeof(char));
			
            while(read(STDIN_FILENO,read_buf,1) > 0)
            {
                buf[j++] = read_buf[0];
				
                if(nFlag && read_buf[0] == '\n')
                {
                    if(write(STDOUT_FILENO,buf,strlen(buf)) == -1)
                        err_sys("write error");
					
                    memset(buf, 0, j);
                    j=0;
                }
            }
			
            if(write(STDOUT_FILENO,buf,strlen(buf)) == -1)
                err_sys("write error");
			
            exit(0);
        }
		
        if((nFlag && line_number > 0) || (cFlag && char_number > 0))
        {
            writeOutput(STDIN_FILENO);
            exit(0);
        }
        else
        {
            int counter = 0;
            int j = 0, temp = 0;
            int total_lines = 0;
            int new_char_number = 0;
            int new_line_number = 0;
            char *buf = malloc(1024 * sizeof(char));
			
            while(read(STDIN_FILENO,read_buf,1) > 0)
            {
                buf[j++] = read_buf[0];
                if(read_buf[0] == '\n')
                {
                    total_lines++;
                }
            }
			
            if(buf[j-1] != '\n')
                total_lines++;
			
            if(nFlag)
            {
                if((temp=line_number+total_lines) > 0)
                    new_line_number = temp;
				
                j=0;
                while(counter < new_line_number)
                    if(buf[j++] == '\n')
                        counter++;
				
                buf[j] = '\0';
				
                if(write(STDOUT_FILENO,buf,strlen(buf)) == -1)
                    err_sys("write error");
            }
			
            if(cFlag)
            {
                if((temp=strlen(buf)+char_number) > 0)
                    new_char_number = temp;
				
                buf[new_char_number] = '\0';
				
                if(write(STDOUT_FILENO,buf,strlen(buf)) == -1)
                    err_sys("write error");
            }
			
            free(buf);
            exit(0);
        }
    }

    if(argc - optind > 0)
        for(int index = optind; index < argc; index++)
        {
            if(strcmp(argv[index],"-") == 0)
            {
                if(argc - optind > 1)
                {
                    char *std = "==> standard input <==\n";
                    if(first_print == true)
                        if(write(STDOUT_FILENO,"\n",1) < 0)
                            err_sys("write error");

                    if(write(STDOUT_FILENO,std,strlen(std)) < 0)
                        err_sys("write error");
                }

                writeOutput(STDIN_FILENO);
                first_print = true;
            }
            else
            {
                char *header = malloc(strlen(argv[index])*5);
                snprintf(header,strlen(argv[index])*5,"==> %s <==\n",argv[index]);
    
                if(lstat(argv[index],&status) < 0)
                    status.st_mode = 0;

            
                if((fd = open(argv[index],O_RDONLY)) < 0)
                    err_ret("cannot open '%s' for reading",argv[index]);

                else 
                    if(read(fd,read_buf,0) < 0)
                    {
                        if(argc - optind > 1)
                        {
                            if(first_print == true)
                                if(write(STDOUT_FILENO,"\n",1) < 0)
                                    err_ret("write error");

                            if(write(STDOUT_FILENO,header,strlen(header)) < 0)
                                err_ret("write error");

                            first_print = true;
                        }

                        err_ret("error reading '%s'",argv[index]);
                    }

                if(fd > 0 && !S_ISDIR(status.st_mode))
                {
                    if(argc - optind > 1)
                    {
                        if(first_print == true)
                            if(write(STDOUT_FILENO,"\n",1) < 0)
                                err_ret("write error");

                        if(write(STDOUT_FILENO,header,strlen(header)) < 0)
                            err_ret("write error");
                    }
                       
                    writeOutput(fd);
                    close(fd);
                    all_wrong = false;
                    first_print = true;
                }

                free(header);
            
            }
        }
                    

    if(all_wrong == true)
        exit(EXIT_FAILURE);
    else
        exit(0);
}

void tryParse(char *arg)
{
    char *errstr;
    long number = strtol(arg,&errstr,10);

    if(errstr[0] == '\0')
    {
        if(arg[0] == '-')
        {
            signed_zero = true;
        }

        if(cFlag)
        {
            char_number = number;
        }

        if(nFlag)
            line_number = number;
    }

    else
    {
        if(arg[0] == '-')
            arg++;

        if(nFlag)
            err_msg("invalid number of lines: ‘%s’",arg);

        if(cFlag)
            err_msg("invalid number of bytes: ‘%s’",arg);

        exit(EXIT_FAILURE);
    }
}

void writeOutput(int fd)
{
    int input_counter = 0;
    int char_counter = 0;
    int j = 0;
    int total_lines = 0;
    int new_char_number = 0;
    int new_line_number = 0;
    int temp = 0;

    // if cFlag is set & char_number is negative,
    // ignore that characters from the last.
    if(cFlag && (char_number < 0 || signed_zero))
    {
        off_t last = lseek(fd, 0, SEEK_END);

        if((temp=char_number+last) > 0)
            new_char_number = temp;

        lseek(fd, 0, SEEK_SET);
    }
    else
        new_char_number = char_number;

    // if nFlag is set & line_number is negative
    // ignore that many lines from last.    
    if(nFlag && (line_number < 0 || signed_zero))
    {
        while(read(fd,read_buf,1) > 0)
            if(read_buf[0] == '\n')
                total_lines++;
            
        if((temp=line_number+total_lines) > 0)
            new_line_number = temp;

        lseek(fd, 0, SEEK_SET);
    }
    else
        new_line_number = line_number;

    while(read(fd,read_buf,1) > 0)
    {
        if(cFlag)
        {
            if(new_char_number == 0) break;
	    
            char_counter ++;
        }

        write_buf[j] = read_buf[0];
        j++;
        
        if(read_buf[0] == '\n' || j == BUFFSIZE - 1)
        {
            if(write(STDOUT_FILENO,write_buf,strlen(write_buf)) == -1)
                err_sys("write error");

            if(read_buf[0] == '\n')
                input_counter ++;
            
            for(int ind = 0; ind <= j; ind++)
                write_buf[ind] = '\0';
            
            j = 0;
        }

        if(input_counter == new_line_number && nFlag)
            break;

        if(char_counter == new_char_number && cFlag)
        {
            if(write(STDOUT_FILENO,write_buf,strlen(write_buf)) == -1)
                err_sys("write error");
            break;
        }

    }
}
