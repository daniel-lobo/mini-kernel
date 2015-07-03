#ifndef _STDLIB_H
#define _STDLIB_H

#define NULL (void *)0
#define METADATA_SIZE sizeof(struct block)
#define MINBLOCK_SIZE 4
#define ENOMEM -1

typedef struct block *type_block;
typedef unsigned int size_t;

struct block{
	int size;
	type_block next;
	type_block prev;
	int free;
};

int     rand(void);
void    srand(unsigned int seed);

int     atof(const char *nptr);
int     atoi(const char *nptr);
long    atol(const char *nptr);
unsigned int uatoi(const char *str);
unsigned int atoh(const char *str);
int pow(int base, int exponent);

char	*itoa(int value, char *str, int base);
char	*utoa(unsigned int value, char *str, int base);

int     abs(int x);
long    labs(long x);

void * malloc(int size);
type_block findBlock(type_block *lastBlock, int size);
type_block splitBlock(type_block b, int size);
type_block expandHeap(type_block lastBlock, int size);
type_block getBaseBlock();

void free(void * address);
int validAddress(void * address);
type_block mergeFreeBlocks(type_block prev, type_block blockToFree);
type_block getBlockFromAddress(void * address);

#endif
