include ../Makefile.inc

HEADERS=-I./include
LIBRARIES=-L../syscallModule/lib
all: libc

libc:
	$(GCC) $(GCCFLAGS) $(HEADERS) $(LIBRARIES) -lsyscall -c *.c
	$(AR) rcs lib/liblibc.a *.o

clean:
	rm -rf *.o lib/*.a

.PHONY: all clean