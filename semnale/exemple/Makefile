#
# Makefile pentru lucrarea de laborator Sisteme de Operare 8
#

CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LDLIBS=liblab.a

CFLAGS	= -Wall -Wextra -O -g -D_FORTIFY_SOURCE=2

LIB	= liblab.a

TEMPFILES = core core.* *.o temp.* *.out typescript*

PROGS =	sigusr reenter child read1 read2 tsleep1 tsleep2 \
        critical mask suspend1 suspend2 \

AR = ar
LIBLAB = liblab.a
RANLIB = ranlib

OBJS = error.o  prmask.o

all:	${PROGS}

lib:	${OBJS}
	${AR} rv ${LIBLAB} $?
	${RANLIB} ${LIBLAB}


#systest1.o:
#		${NOANSICOMPILE.c} systest1.c

# We want to link with our own version of system() for systest[12].
#systest1: system.o systest1.o
#		  $(LINK.c) -o $@ systest1.o system.o $(LDLIBS)
#
#systest2: system.o systest2.o
#		  $(LINK.c) -o $@ systest2.o system.o $(LDLIBS)


tsleep1:  sleep1.o tsleep1.o
		  $(LINK.c) -o $@ sleep1.o tsleep1.o $(LDLIBS)

tsleep2:  sleep2.o tsleep2.o
		  $(LINK.c) -o $@ sleep2.o tsleep2.o $(LDLIBS)



clean:
	rm -f ${PROGS} ${LIBLAB} ${TEMPFILES}
