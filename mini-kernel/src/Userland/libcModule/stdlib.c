#include "./include/stdlib.h"
#include <stdint.h>
#include "./include/string.h"
#include "./include/ctype.h"
#include "../syscallModule/include/syscall.h"
#define align(x) ((x+3)&(~3))

int errno;
unsigned long rand_seed = 4;
void * baseHeapAddress;
void * lastSbrk=NULL;

int
rand(void)
{
    rand_seed = rand_seed * 1103515245 + 12345;
    return (unsigned int)(rand_seed / 65536) % 32768;
}

void
srand(unsigned int seed)
{
    rand_seed = seed;
}

int
abs(int number)
{
    return (number < 0) ? -number : number;
}

long
labs(long number)
{
    return (number < 0) ? -number : number;
}

int
atoi(const char *str)
{
    int result = 0;
    while (*str != 0)
    {
        if (!isdigit(*str))
        {
           errno = 22;
           return 0;
        }
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

char *
itoa(int value, char *s, int base)
{
	int numeral, i = 0;
    char *sb = s;

    if (base == 10 && value < 0)
    {
        *s++ = '-';
        value = -value;
    }

	if (base >= 2 && base <= 36)
	{
		do {
			numeral = value % base;
			value = value/base;
			if (numeral >= 0 && numeral <= 9)
			{
				s[i] = '0' + numeral;
			}		
			else
			{
				s[i] = 'A' + numeral - 10;
			}
			i++;
		}
		while (value);
	}

	s[i] = '\0';

    strrev(s);
	
    return sb;
}

char * utoa(unsigned int value, char *s, int base)
{
	unsigned int numeral, i = 0;
    char *sb = s;

	if (base >= 2 && base <= 36)
	{
		do
		{
			numeral = value % base;
			value = value/base;
			if (numeral >= 0 && numeral <= 9)
			{
				s[i] = '0' + numeral;
			}
			else
			{
				s[i] = 'A' + numeral - 10;
			}
			i++;
		}
		while (value);
	}

	s[i] = '\0';

    strrev(s);

    return sb;
}

void * malloc(int size){
	type_block	currentBlock,lastBlock;
	int 	alignedSize;
	alignedSize=align(size);
	
	if(baseHeapAddress){
		//Find a block starting from base address
		lastBlock=baseHeapAddress;
		currentBlock=findBlock(&lastBlock,alignedSize);
		if(currentBlock){
		//Try to split the current block, using what is just necessary
			if((currentBlock->size - alignedSize)>=(HEADERBLOCK_SIZE+4)){
				splitBlock(currentBlock,alignedSize);
			}
			currentBlock->free=0;
		}else{
		// Expand heap
			currentBlock= expandHeap(lastBlock,alignedSize);
			if(!currentBlock){
			//Couldn't expand heap	
				return NULL;
			}
		}
	}else{
		//First initialization
		baseHeapAddress=sc_getHeapAddress();
		currentBlock = expandHeap(baseHeapAddress, alignedSize);
		if(!currentBlock){
			//Couldn't expand heap
			return NULL;
		}
		baseHeapAddress=currentBlock;
	}
	return currentBlock->data;
}

type_block findBlock(type_block *lastBlock,int size){
	type_block block=lastBlock;
	while(block && !(block->free && block->size >=size)){
		*last=block;
		block=block->next;
	}
	return (block);
}

type_block splitBlock(type_block b,int size){
	type_block splitedBlock;
	splitBlock= b->data+ size;
	splitBlock->next = b->next;
	splitBlock-> prev = b;
	splitBlock->free = 1;
	splitBlock->dataPointer= splitBlock->data;
	splitBlock->size = b->size-size-HEADERBLOCK_SIZE;
	b->next= splitBlock;
	b->size= size;
	
	if(splitBlock->next){
		splitBlock->next->prev= splitBlock;
	}
}

type_block expandHeap(block lastBlock,int size){
	int sb;
	type_block b;
	int flag=lastSbrk==NULL;
	
	while(lastblock+size+ HEADERBLOCK_SIZE > lastSbrk){
		lastSbrk=sc_sbrk();
	}
	
	b=lastBlock;
	if(!flag){
		b->prev=lastBlock->prev;
	}else{
		b->prev=NULL;
	}
	b->free=0;
	b->dataPointer=b->data;
	b->size=lastSbrk-lastBlock-HEADERBLOCK_SIZE;
	splitBlock(b,size);
	
	return (b);	
}	

void * free(void *address){
	type_block blockToFree;
	if(validAddress(address)){
		blockToFree=getBlockFromAddress(address);
		blockToFree->free=1;
		//Try to merge with next or previous block if they're free to avoid fragmentation
		if(blockToFree->prev && blockToFree->prev->free){
			blockToFree= mergeFreeBlocksBlocks(blockToFree->prev,blockToFree);
		}
		if(blockToFree->next){
			if(blockToFree->next->free){
				blockToFree=mergeFreeBlocksBlocks(blockToFree,blockToFree->next);
			}
		}else{
			//Check if the resultant block is the only one and reset baseHeapAddress
			if (blockToFree->prev){
				blockToFree->prev->next = NULL;
			}else{
				baseHeapAddress = NULL;
			}
		}
	}
}
type_block mergeFreeBlocks(t_block prev,t_block next){
	//Merges the block
	if (b->next && b->next ->free ){
		prev->size += HEADERBLOCK_SIZE + next->size;
		prev->next = next->next;
		if (prev->next)
			prev->next ->prev = b;
	}
	return (prev);
 }
 
 int validAddress(void * address){
		if(baseHeapAddress){
			if(address>baseHeapAddress && address< lastSbrk){
				return (address==getBlockFromAddress(address)->dataPointer);
			}
		}
		return (0);
}
type_block getBlockFromAddress(void * address){
		//address should not be modified
		char * ptr;
		ptr =address;
		return (address=ptr-=HEADERBLOCK_SIZE);
}

	
