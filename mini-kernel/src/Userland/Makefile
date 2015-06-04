include Makefile.inc

all: syscallModule libcModule shellModule

shellModule:
	cd shellModule; make

libcModule:
	cd libcModule; make

syscallModule:
	cd syscallModule; make

clean:
	cd shellModule; make clean
	cd syscallModule; make clean
	cd libcModule; make clean
	rm -rf *.bin

.PHONY: syscallModule libcModule shellModule all clean
