#include "defs.h"
#include <stdint.h>

void _outb(unsigned char value, unsigned short int port);
int _inb(unsigned short int port);

void _set_handlers(void);
void _enable_pic(void);