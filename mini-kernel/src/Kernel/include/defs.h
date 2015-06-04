#ifndef _defs_
#define _defs_

#pragma pack (1) 		/* Alinear estructuras a 1 byte */

/* IDTR  */
typedef struct {
  unsigned int  limit;
  unsigned long long int base;
} IDTR;     

#endif

