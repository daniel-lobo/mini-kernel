
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

unsigned int atoh(const char *str)
{
    int result, index;
    result = index = 0;

    while (str[index] != 0)
    {
        if (isxdigit(str[index])){
        	result = result + (str[index] - 55) * pow(16, index);
        }else if(isdigit(str[index])){
        	result = result + (str[index] - '0') * pow(16, index);
        }else{
           return 0;
        }
        index++;
    }
    return result;
}

int pow(int base, int exponent){
	int result = 1;
	while(exponent > 0){
		result *= base;
		exponent--;
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
	if(size < 1){
		return null;
	}

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
			// Expand heap because it didnt find any available block
			currentBlock = expandHeap(lastBlock, alignedSize);
			if(!currentBlock){
				//Couldn't expand heap	
				return NULL;
			}
		}
	}else{
		//First initialization
		baseHeapAddress = lastSbrk = (type_block)sc_getBaseHeapAddress();
		currentBlock = expandHeap(NULL, alignedSize);
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
	//My future base address for the block
	char * neededBaseAdd;
	if (lastBlock){
		neededBaseAdd = (char *)(lastBlock + 1) + lastBlock->size;
	}else{
		neededBaseAdd = (char *)baseHeapAddress;
	}

	//Sbrk till it can fit the block or no mem space
	while(neededBaseAdd + METADATA_SIZE + size > (char *)lastSbrk){
		void * tmp = sc_sbrk();
		if(tmp == (void *)ENOMEM){
			return NULL;
		}else{
			lastSbrk = tmp;
		}
	}
	
	type_block newBlock;
	newBlock = (type_block)neededBaseAdd;
	newBlock->size = size;
	newBlock->free = 0;
	newBlock->prev = lastBlock;
	if(lastBlock){
		lastBlock->next = newBlock;
	}else{
		baseHeapAddress = newBlock;
	}
	return newBlock;
	//printf("lastSbrk:%u lastBlock:%u lastBlock+1:%u", lastSbrk, lastBlock, lastBlock+1);
	//lastBlock->free = 0;
	//lastBlock->size = (int)((char *)lastSbrk - ((char *)lastBlock + METADATA_SIZE));
	//splitBlock(lastBlock, size);	
}	

type_block getBaseBlock(){
	return baseHeapAddress;
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
			//If it was the last block, make sure prev points to null
			if (blockToFree->prev){
				blockToFree->prev->next = NULL;
			}else{
				//Has no next nor previous so heap is empty
				baseHeapAddress = NULL;
			}
		}
	}
}

type_block mergeFreeBlocks(type_block prev, type_block blockToFree){
	//Merges the block
	prev->size += blockToFree->size;
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
		//In a trully protectec system we would throw a sc exception here
		return 0;
}

type_block getBlockFromAddress(void * address){
		//get block from data pointer
		return (type_block)((char *)address - METADATA_SIZE);
}

	
