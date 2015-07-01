#ifndef _STDLIB_H
#define _STDLIB_H
#define HEADERBLOCK_SIZE 
struct block{
	int size;
	block * next;
	block * prev;
	int free;
	void* dataPointer;
	char data[1];	
}typedef struct block *type_block
int     rand(void);
void    srand(unsigned int seed);

int     atof(const char *nptr);
int     atoi(const char *nptr);
long    atol(const char *nptr);

char	*itoa(int value, char *str, int base);
char	*utoa(unsigned int value, char *str, int base);

int     abs(int x);
long    labs(long x);

#endif
