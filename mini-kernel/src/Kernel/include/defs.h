/***************************************************
  Defs.h
	
****************************************************/

#ifndef _defs_
#define _defs_

#define byte unsigned char
#define word short int
#define dword int

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )


#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Interrupt descriptor */
typedef struct {
  uint16_t  offset_l,
            selector;
  uint8_t   cero,
            access;
  uint16_t  offset_m;
  uint32_t  offset_h,
            cero;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;     

#endif

