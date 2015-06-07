include Makefile.inc

all: syscallModule libcModule shellModule sampleDataModule

shellModule:
	cd shellModule; make

libcModule:
	cd libcModule; make

syscallModule:
	cd syscallModule; make

sampleDataModule:
	printf "This is sample data." >> SampleDataModule.bin && dd if=/dev/zero bs=1 count=1 >> SampleDataModule.bin

clean:
	cd shellModule; make clean
	cd syscallModule; make clean
	cd libcModule; make clean
	rm -rf *.bin

.PHONY: syscallModule libcModule shellModule all clean
