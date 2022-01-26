/* Our own header, to be included *after* all standard system headers */

#ifndef __LAB_H__
#define __LAB_H__

#include    <sys/types.h>   /* required for some of our prototypes */
#include    <stdio.h>       /* for convenience */
#include    <stdlib.h>      /* for convenience */
#include    <string.h>      /* for convenience */
#include    <unistd.h>      /* for convenience */

#define MAXLINE 4096            /* max line length */

#define FILE_MODE   (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
                    /* default file access permissions for new files */
#define DIR_MODE    (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
                    /* default permissions for new directories */


                    /* prototypes for our own functions */
char    *path_alloc(int *);         /* {Prog pathalloc} */
int      open_max(void);            /* {Prog openmax} */
void     clr_fl(int, int);          /* {Prog setfl} */
void     set_fl(int, int);          /* {Prog setfl} */

void    err_dump(const char *, ...);    /* {App misc_source} */
void    err_msg(const char *, ...);
void    err_quit(const char *, ...);
void    err_ret(const char *, ...);
void    err_sys(const char *, ...);

#endif  /* __LAB_H__ */
