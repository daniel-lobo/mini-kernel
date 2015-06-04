include ../../Makefile.variables
include ../Makefile.inc

MODULE=shellCodeModule.bin
SOURCES=shellModule.c $(wildcard commands/*.c)
HEADERS=-I../../headers -I../clibs/include -I./
LIBRARIES=-L../syscalls/lib -L../clibs/lib

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T ./ld/shell.ld $(SOURCES) $(HEADERS) $(LIBRARIES) -lc -lsyscalls -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean