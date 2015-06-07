include ../Makefile.inc

MODULE = sampleCodeModule.bin
SOURCES = shell.c commands.c
HEADERS = -I./include -I../libcModule/include -I../syscallModule/include
LIBRARIES = -L../libcModule/lib -L../syscallModule/lib

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T shell.ld $(SOURCES) $(HEADERS) $(LIBRARIES) -llibc -lsyscall -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
