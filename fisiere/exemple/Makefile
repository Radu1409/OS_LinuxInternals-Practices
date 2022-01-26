#
#Makefile pentru lucrarea de laborator Sisteme de Operare 1
#

CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LDLIBS=liblab.a

CFLAGS	= -Wall -Wextra -O -g -D_FORTIFY_SOURCE=2

LIB	= liblab1.a

TEMPFILES = core core.* *.o temp.* *.out typescript*

PROGS =	seek hole fileflags filetype access umask changemod unlink zap ftw4 mycd cdpwd devrdev

AR = ar
LIBLAB = liblab.a
RANLIB = ranlib

OBJS = error.o pathalloc.o

all:	${PROGS}

lib:	${OBJS}
	${AR} rv ${LIBLAB} $?
	${RANLIB} ${LIBLAB}

clean:
	rm -f ${PROGS} ${TEMPFILES} file.hole
