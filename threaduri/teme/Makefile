#
# Makefile pentru exerciții
#

CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -pthread


CFLAGS	= -Wall -O -I$(dir $(LIBLAB))

TEMPFILES = core core.* *.o temp.* *.out typescript*

all:
	@echo "Please compile directly the intended programs by name"

clean:
	rm -f ${TEMPFILES}
