#ifndef _MMU_
#define _MMU_

//Hasta 5 MB de heap
#define MAX_HEAP_SIZE 0x2800000
//Incrementos de 8KB
#define SBRK_SIZE 0x10000
#define ENOMEM -1

void * sbrk_handler();
void * bhadd_handler();
void init_heap(void * address);

#endif
