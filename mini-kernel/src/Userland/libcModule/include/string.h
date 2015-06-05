#ifndef _STRING_H
#define _STRING_H

char   *strcpy(char *dest, const char *src);
char   *strncpy(char *dest, const char *src, uint64_t n);

char   *strcat(char *dest, const char *src);

uint64_t strlen(const char *s);

int     strcmp(const char *s1, const char *s2);
int     strncmp(const char *s1, const char *s2, uint64_t n);

char   *strrev(char *str);

char   *strtoupper(char *str);
char   *strtolower(char *str);

char   *strchr(const char *s, int c);
char   *strrchr(const char *s, int c);

void   *memcpy(void *dest, const void *src, uint64_t n);
int     memcmp(const void *dest, const void *src, uint64_t n);
void   *memset(void *s, int c, uint64_t n);

void    trim(char *str);

#endif
