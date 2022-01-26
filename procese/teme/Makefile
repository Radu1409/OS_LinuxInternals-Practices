#
# Makefile pentru exerciții (compilează și biblioteca din exemple dacă e nevoie)
#

CC=gcc
COMPILE.c=$(CC) $(CFLAGS) $(CPPFLAGS) -c
LINK.c=$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LIBLAB=../exemple/liblab.a
LDLIBS=$(LIBLAB)

CFLAGS	= -Wall -Wextra -O -g -D_FORTIFY_SOURCE=2 -I$(dir $(LIBLAB))

TEMPFILES = core core.* *.o temp.* *.out typescript*

all:
	@echo "Please compile directly the intended programs by name"

lib:
	$(MAKE) -C $(dir $(LIBLAB)) lib

clean:
	rm -f ${PROGS} ${TEMPFILES}
