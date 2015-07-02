
#include <stdint.h>
#include "./include/stdlib.h"
#include "./include/string.h"
#include "./include/ctype.h"
#include "../syscallModule/include/syscall.h"

#define align(x) ((x+3)&(~3))

int errno;
unsigned long rand_seed = 4;
type_block baseHeapAddress;
void * lastSbrk;

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
	type_block currentBlock, lastBlock;
	int alignedSize = align(size);
	
	if(baseHeapAddress){
		//Find a block starting from base address
		lastBlock = baseHeapAddress;
		currentBlock = findBlock(&lastBlock, alignedSize);
		if(currentBlock){
			//Try to split the current block, using what is just necessary
			if(currentBlock->size - alignedSize >= METADATA_SIZE + MINBLOCK_SIZE){
				splitBlock(currentBlock, alignedSize);
			}
			currentBlock->free = 0;
		}else{
			// Expand heap
			currentBlock = expandHeap(lastBlock, alignedSize);
			if(!currentBlock){
				//Couldn't expand heap	
				return NULL;
			}
		}
	}else{
		//First initialization
		baseHeapAddress = lastSbrk = (type_block)sc_getBaseHeapAddress();
		currentBlock = expandHeap(baseHeapAddress, alignedSize);
		if(!currentBlock){
			//Couldn't expand heap
			return NULL;
		}
	}
	return (void *)(currentBlock + 1);
}

type_block findBlock(type_block *lastBlock, int size){
	type_block curBlock = baseHeapAddress;
	while(curBlock && !(curBlock->free && curBlock->size >= size)){
		*lastBlock = curBlock;
		curBlock = curBlock->next;
	}
	return curBlock;
}

type_block splitBlock(type_block b, int size){
	type_block newBlock;
	newBlock = (type_block)((char *)(b + 1) + size);
	newBlock->next = b->next;
	newBlock->prev = b;
	newBlock->free = 1;
	newBlock->size = b->size - size - METADATA_SIZE;
	b->next = newBlock;
	b->size = size;
	
	if(newBlock->next){
		newBlock->next->prev= newBlock;
	}

	return newBlock;
}

type_block expandHeap(type_block lastBlock, int size){	
	while((char *)(lastBlock + 1) + size > (char *)lastSbrk){
		lastSbrk = sc_sbrk();
		if (lastSbrk == (void *)ENOMEM){
			return NULL;
		}
	}

	lastBlock->free = 0;
	lastBlock->size = (int)((char *)lastSbrk - ((char *)lastBlock + METADATA_SIZE));
	splitBlock(lastBlock, size);
	
	return lastBlock;	
}	

void free(void * address){
	type_block blockToFree;
	if(validAddress(address)){
		blockToFree = getBlockFromAddress(address);
		blockToFree->free = 1;
		//Try to merge with next or previous block if they're free to avoid fragmentation
		if(blockToFree->prev && blockToFree->prev->free){
			blockToFree = mergeFreeBlocks(blockToFree->prev, blockToFree);
		}
		if(blockToFree->next){
			//Check wether the next one is also free for a merge
			if(blockToFree->next->free){
				blockToFree = mergeFreeBlocks(blockToFree, blockToFree->next);
			}
		}else{
			//If it was the last block, make it go boom
			if (blockToFree->prev){
				blockToFree->prev->next = NULL;
			}
		}
	}
}

type_block mergeFreeBlocks(type_block prev, type_block blockToFree){
	//Merges the block
	prev->size += METADATA_SIZE + blockToFree->size;
	prev->next = blockToFree->next;
	if (prev->next){
		prev->next->prev = blockToFree;
	}
	return prev;
 }
 
int validAddress(void * address){
		if(baseHeapAddress){
			if(address >= (void *)baseHeapAddress && address < (void *)lastSbrk){
				return address == (getBlockFromAddress(address) + 1);
			}
		}
		return 0;
}

type_block getBlockFromAddress(void * address){
		//get block from data pointer
		return (type_block)((char *)address - METADATA_SIZE);
}

	
