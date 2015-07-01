#ifndef _MMU_
#define _MMU_

#define MAX_HEAP_SIZE 0x500000
#define SBRK_SIZE 0xFA00
#define ENOMEM -1

void * sbrk_handler();
void init_heap(void * address);

#endif
