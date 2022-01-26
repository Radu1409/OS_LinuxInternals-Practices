#
#Makefile pentru lucrarea de laborator Sisteme de Operare 2
#

CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LDLIBS=liblab.a

CFLAGS	= -Wall -Wextra -O -g -D_FORTIFY_SOURCE=2

LIB	= liblab.a

TEMPFILES = core core.* *.o temp.* *.out typescript* testlab

PROGS =	doatexit echoarg testjmp getrlimit \
        fork1 vfork1 wait1 fork2 tellwait1 tellwait2 exec1 echoall \
        exec2 systest1 systest3 pruids test1 times1 echoarg \
        orphan

AR = ar
LIBLAB = liblab.a
RANLIB = ranlib

OBJS = error.o prexit.o tellwait.o

all:	${PROGS}

lib:	${OBJS}
	${AR} rv ${LIBLAB} $?
	${RANLIB} ${LIBLAB}

clean:
	rm -f ${PROGS} ${LIBLAB} ${TEMPFILES}
