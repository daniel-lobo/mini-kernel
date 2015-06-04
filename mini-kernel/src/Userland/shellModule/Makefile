include ../Makefile.inc

MODULE = shellCodeModule.bin
SOURCES = $(wildcard *.c)
HEADERS = -I./include -I../libcModule/include -I../syscallModule/include
LIBRARIES = -L../syscallModule/lib -L../libcModule/lib

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T ./ld/shell.ld $(SOURCES) $(HEADERS) $(LIBRARIES) -lc -lsyscalls -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
