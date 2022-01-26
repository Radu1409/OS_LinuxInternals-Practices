#include    <errno.h>       /* for definition of errno */
#include    <stdarg.h>      /* ANSI C header file */
#include    "lab.h"

static void err_doit(int, const char *, va_list);
static void err_init(int argc, char **argv) __attribute__((constructor));

/* store program name for displaying in error messages */
char    *pname = NULL;

/* startup code, from before calling main, to store argv[0] as program name */
static void err_init(int argc, char **argv)
{
    (void)argc;
    pname = argv[0];
}

/* Nonfatal error related to a system call.
 * Print a message and return. */
void err_ret(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, fmt, ap);
    va_end(ap);
    return;
}

/* Fatal error related to a system call.
 * Print a message and terminate. */
void err_sys(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, fmt, ap);
    va_end(ap);
    exit(1);
}

/* Fatal error related to a system call.
 * Print a message, dump core, and terminate. */
void err_dump(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(1, fmt, ap);
    va_end(ap);
    abort();        /* dump core and terminate */
    exit(1);        /* shouldn't get here */
}

/* Nonfatal error unrelated to a system call.
 * Print a message and return. */
void err_msg(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);
    return;
}

/* Fatal error unrelated to a system call.
 * Print a message and terminate. */
void err_quit(const char *fmt, ...)
{
    va_list     ap;

    va_start(ap, fmt);
    err_doit(0, fmt, ap);
    va_end(ap);
    exit(1);
}

/* Print a message and return to caller.
 * Caller specifies "errnoflag". */
static void err_doit(int errnoflag, const char *fmt, va_list ap)
{
    int     errno_save;
    char    buf[MAXLINE];
    int     pos = 0;

    errno_save = errno; /* value caller might want printed */
    if ( pname )
        pos = snprintf(buf, MAXLINE, "%s: ", pname);
    pos += vsnprintf(buf + pos, MAXLINE - pos, fmt, ap);
    if (errnoflag)
        pos += snprintf(buf + pos, MAXLINE - pos, ": %s", strerror(errno_save));
    if (pos < MAXLINE - 1)
    {
        buf[pos] = '\n';
        buf[pos + 1] = 0;
    }

    fflush(stdout);     /* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(NULL);       /* flushes all stdio output streams */
    return;
}

