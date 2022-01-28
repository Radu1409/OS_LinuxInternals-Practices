#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "lab.h"

/* file type in the enumeration:
 * directories are handled specially */
typedef enum {
    F_NORMAL,
    F_DIR
} f_type;

/* struncture containing information about the files */
typedef struct {
    char *name;
    f_type type;
    int error;
    size_t size;
} info_t;

void write_stdin(int fd);

/* display option flags, lines, words, bytes */
bool lFlag = 0;
bool wFlag = 0;
bool cFlag = 0;
bool LFlag = 0;

int main(int argc, char** argv)
{
    char buffer[10];
    struct stat st;
    int option;
    int width = 0; // alignment width for displayed counts
    int numFiles;  // number of files to be counted
    int numFields; // number of options to be counted
    info_t *files; // count values for each file
    info_t *file;  // pointer for cycling through files
    size_t *sizes; // vector with counted values
    size_t *size;  // pointer for cycling through sizes
    int i, j;
    bool found_error = false;
    
    // check for optional arguments
    while((option = getopt(argc,argv,"lwcL")) !=-1)
	    switch(option)
	    {
		    case 'l':
			    lFlag = 1;
			    break;
		    case 'w':
			    wFlag = 1;
			    break;
		    case 'c':
			    cFlag = 1;
			    break;
		    case 'L':
			    LFlag = 1;
			    break;
		    case '?':
			    exit(EXIT_FAILURE);
	    }

    // remaining argument are the files to be counted
    numFiles = argc - optind;

    // if no display option was present in the command line, display every counted value
    if (!wFlag && !lFlag && !cFlag && !LFlag)
    {
        wFlag = 1;
        lFlag = 1;
        cFlag = 1;
    }
    
    if (numFiles < 1)
    {
        write_stdin(STDIN_FILENO);
    }

    numFields = wFlag + lFlag + cFlag + LFlag;

    // allocate memory for the counts
    files = malloc(sizeof(info_t) * (numFiles + (numFiles > 1)));
    sizes = malloc(sizeof(size_t) * numFields * (numFiles + (numFiles > 1)));

    // go through all the files
    for(i=optind, file=files, size=sizes; i<argc; i++, file++)
    {
        int byte_count=0;
        int word_count = 0;
        int line_count = 0;
        char previous[10]; // TODO: this is strange
        int fd;
        int max_length = -1;
        int curr_line_char = 0;

        previous[0]=' ';

        file->name = argv[i];
        if((fd=open(file->name,O_RDONLY)) < 0 )
        {
            file->error = errno;
            file->size = 0;
            file->type = F_NORMAL;
            if (lFlag) *size++ = 0;
            if (wFlag) *size++ = 0;
            if (cFlag) *size++ = 0;
            if (LFlag) *size++ = 0;
            found_error = true;
            continue;
        }
        else
        {
            fstat(fd, &st);

            if(S_ISDIR(st.st_mode))
            {
                file->size = 0;
                file->type = F_DIR;
            }
            else
            {
                file->size = st.st_size;
                file->type = F_NORMAL;
            }

        }

        while(read(fd,buffer,1) >0)
        {
            // TODO: some of the counts in binary files are wrong
            byte_count++;
            // keep the count of characters per line
            // this will reset for newline
            curr_line_char++;

            if(buffer[0]=='\n')
            {
                line_count++;
                // max_length will store longest line size
                // if current line char count is greter, store it in max length
                if(LFlag && curr_line_char > max_length)
                    max_length = curr_line_char;

                // reset per line count
                curr_line_char = 0;
            }
            if(isspace(buffer[0])==0 && isspace(previous[0])!=0)
                word_count++;

            previous[0]=buffer[0];
        }

        if (lFlag) *size++ = line_count;
        if (wFlag) *size++ = word_count;
        if (cFlag) *size++ = byte_count;
        // save max length ignoring last newline character
        if (LFlag) *size++ = max_length - 1;

        close(fd);
    }

    // count the totals
    if ( numFiles > 1 )
    {
        size = sizes;
        for (i = 0; i < numFiles; i++)
        {
            for (j = 0; j < numFields; j++)
            {
                // if LFlag is set & last field is of max length
                // save max value in total calculation
                if (LFlag && j==numFields-1)
                {
                    if (sizes[i*numFields+j] > sizes[numFiles * numFields + j])
                        sizes[numFiles * numFields + j] = *size++;
                }
                else
                    sizes[numFiles * numFields + j] += *size++;
            }
            files[numFiles].size += files[i].size;
        }
        // mark the last line as having the name total
        files[numFiles].name = "total";
        files[numFiles].error = 0;
        files[numFiles].type = F_NORMAL;
        // and increment the numFiles, so the total line will also be displayed
        numFiles++;
    }

    // align sizes to largest value (normally the byte count is the largest)
    // largest_byte_count = sizes[2]
    // when displaying only one value do not align at all
    if (numFields > 1 || numFiles > 1)
    {
        width = snprintf(NULL, 0, "%ld", files[numFiles - 1].size);
        // when there is at least one directory among the files, align at least 7 characters
        for (i = 0; i < numFiles; i++)
            if  (files[i].type == F_DIR && width < 7)
                width = 7;
    }

    // display the counts
    for (i = 0, size = sizes, file=files; i < numFiles; i++, size += numFields, file++)
    {
        if (file->error)
        {
            errno = file->error;
            err_ret("%s", file->name);
            continue;
        }
        if (file->type == F_DIR)
        {
            err_msg("%s: Is a directory", files->name);
            found_error = true;
        }

        for (j = 0; j < numFields; j++)
            printf("%*ld ", width, size[j]);
        printf("%s\n", file->name);
    }

    free(sizes);
    free(files);

    if(found_error)
        exit(1);
}

void write_stdin(int fd)
{
    int line_no = 0;
    int byte_no = 0;
    int word_no = 0;
    int max_length = -1;
    int width = 0;
    int status = 0;
    int curr_line_char = 0;
    size_t *sizes;
    size_t *size;
    char buf[10];
    char previous[10];

    previous[0]=' ';
    struct stat st;

    int Flag = lFlag + wFlag + cFlag + LFlag;
    sizes = malloc(sizeof(size_t) * Flag);

    while (read(fd,buf,1) > 0)
    {
        // TO DO: some of the counts in binary files are wrong
        byte_no++;
        // keep the count of characters per line
        // this will reset for newline
        curr_line_char++;

        if(buf[0]=='\n')
        {
            line_no++;
            // max_length will store longest line size
            // if current line char count is greter, store it in max length
            if(LFlag && curr_line_char > max_length)
                max_length = curr_line_char;

            // reset per line count
            curr_line_char = 0;
        }

        if(isspace(buf[0])==0 && isspace(previous[0])!=0)
            word_no++;

        previous[0]=buf[0];
    }
    size = sizes;

    if (lFlag) *size++ = line_no;
    if (wFlag) *size++ = word_no;
    if (cFlag) *size++ = byte_no;
    if (LFlag) *size++ = max_length - 1;
        
	
    if (Flag > 1)
    {
        width = snprintf(NULL, 0, "%d", byte_no);
    }
    size = sizes;
    fstat(fd, &st);

    if(S_ISDIR(st.st_mode))
    {
        err_msg("'standard input': Is a directory");
        width = 7;
        status = 1;
    }

    // display the counts    
    if (isatty(STDIN_FILENO))
    {
        for (int j = 0; j < Flag; j++)
        {
            if (Flag == 1)
            {
                printf("%ld", size[j]);
                break;
            }
            if (j != Flag - 1)
                printf("%*ld ", 7, size[j]);
            else
                printf("%*ld", 7, size[j]);
        }
    }
    else
    {
        for (int j = 0; j < Flag; j++)
            if (j != Flag - 1)
                printf("%*ld ", width, size[j]);
            else
                printf("%*ld", width, size[j]);
    }	    
    printf("\n");
    free(sizes);

    close(fd);
    exit(status);
}
