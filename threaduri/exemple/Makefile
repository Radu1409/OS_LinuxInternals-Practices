PROGS 	= hello readwrite semrw

all : $(PROGS)

hello : hello.c
	gcc -o $@ hello.c -lpthread

readwrite : readwrite.c
	gcc -o $@ readwrite.c -lpthread

semrw : semrw.c
	gcc -o $@ semrw.c -lpthread

clean :
	rm -f $(PROGS) *.o *~