include ../Makefile.inc

MODULE = shellCodeModule.bin
SOURCES = $(wildcard *.c)
HEADERS = -I./include -I../libcModule/include -I../syscallModule/include
LIBRARIES = -L../libcModule/lib -L../syscallModule/lib

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shell.ld $(SOURCES) $(HEADERS) $(LIBRARIES) -llibc -lsyscall -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
