#include "./include/mmu.h"
#include "./include/syscall.h"

void * start_address;
void * cur_address;

void * sbrk_handler(){
	if ((char *)cur_address + SBRK_SIZE > (char *)start_address + MAX_HEAP_SIZE){
		return (void *)ENOMEM;
	} else {
		cur_address = (void *)((char *)cur_address + SBRK_SIZE);
		return cur_address;
	}
}

void * bhadd_handler(){
	return start_address;
}

void init_heap(void * address){
	start_address = cur_address = address;
}