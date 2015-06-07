#include "defs.h"
#include <stdint.h>

void _outb(unsigned char value, unsigned short int port);
int _inb(unsigned short int port);

void _cli();
void _sti();

void _hlt();

void _set_idt_entry(unsigned char index, void * handler, unsigned long int base);

uint64_t _syscall(uint64_t callid, ...);

void _mask_pic();  /* Escribe mascara de PIC1 */

void _get_idtr(IDTR * idtr);

void _pit_handler();
void _keyboard_handler();
void _syscall_handler();
