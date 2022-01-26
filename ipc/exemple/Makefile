#
#Makefile pentru lucrarea de laborator Sisteme de Operare 8
#

CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LDLIBS=liblab.a

CFLAGS	= -Wall -Wextra -O -g -D_FORTIFY_SOURCE=2

LIB	= liblab.a

TEMPFILES = core core.* *.o temp.* *.out typescript*

PROGS =	pipe1 pipe2 popen2 myuclc popen1 add2 pipe4 \
        msgtool semtool shmtool semstat

AR = ar
LIBLAB = liblab.a
RANLIB = ranlib

OBJS = error.o  prmask.o

all:	${PROGS}

lib:	${OBJS}
	${AR} rv ${LIBLAB} $?
	${RANLIB} ${LIBLAB}


clean:
	rm -f ${PROGS} ${LIBLAB} ${TEMPFILES}
