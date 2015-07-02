#include "./include/mmu.h"
#include "./include/syscall.h"

void * start_address;
void * cur_address;

void * sbrk_handler(){
	cur_address += SBRK_SIZE;
	if (cur_address > start_address + MAX_HEAP_SIZE){
		return (void *)ENOMEM;
	} else {
		return cur_address;
	}
}

void * bhadd_handler(){
	return start_address;
}

void init_heap(void * address){
	start_address = address;
	cur_address = start_address;
}