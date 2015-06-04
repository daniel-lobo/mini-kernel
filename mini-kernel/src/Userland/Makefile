include ../Makefile.variables
include Makefile.inc

all: syscalls clibs shell shellDataModule

shell:
	cd shel; make

clibs:
	cd clibs; make

syscalls:
	cd syscalls; make

clean:
	cd shell; make clean
	cd syscalls; make clean
	cd clibs; make clean
	rm -rf *.bin

.PHONY: syscalls clibs shell all clean
