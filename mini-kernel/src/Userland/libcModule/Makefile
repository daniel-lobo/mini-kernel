include ../Makefile.inc

HEADERS = -I./include
LIBRARIES = -L../syscallModule/lib

all: clibs

clibs:
	$(GCC) $(GCCFLAGS) $(HEADERS) $(LIBRARIES) -lsyscalls -c libc.c
	$(AR) rcs lib/libc.a libc.o

clean:
	rm -rf *.o liblib*.a

.PHONY: all clean
