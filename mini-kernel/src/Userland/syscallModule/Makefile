include ../Makefile.inc

all: syscall_int syscalls link

syscalls:
	$(GCC) $(GCCFLAGS) -I ./include -c syscall.c

syscall_int:
	$(ASM) $(ASMFLAGS) syscall.asm -o syscall_asm.o

link:
	$(AR) rcs lib/libsyscall.a syscall_asm.o syscall.o

clean:
	rm -rf *.o lib/*.a

.PHONY: all clean
