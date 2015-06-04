#include "defs.h"

void _outb(unsigned char value, unsigned short int port);
int _inb(unsigned short int port);

void _cli();
void _sti();

void _hlt();

void _set_idt_entry(unsigned char index, void * handler, void * base);

int _syscall(int number, int param1, int param2, int param3, int param4, int param5);

void _mask_pic();  /* Escribe mascara de PIC1 */

void _get_idtr(IDTR * idtr);

void _pit_handler();
void _keyboard_handler();
void _syscall_handler();
