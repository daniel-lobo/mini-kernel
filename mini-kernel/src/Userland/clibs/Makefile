include ../../Makefile.variables
include ../Makefile.inc

HEADERS=-I../../headers -I./include
LIBRARIES=-L../syscalls/lib

all: clibs

clibs:
	$(GCC) $(GCCFLAGS) $(HEADERS) $(LIBRARIES) -lsyscalls -c libc.c
	$(AR) rcs lib/clibs.a clibs.o

clean:
	rm -rf *.o liblib*.a

.PHONY: all clean