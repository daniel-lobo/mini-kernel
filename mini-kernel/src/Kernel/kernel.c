#include <stdint.h>

#include "./include/kasm.h"
#include "./include/defs.h"
#include "./include/keyboard.h"
#include "./include/syscall.h"
//#include "./include/video.h"

#include "../Userland/clibs/include/stdlib.h"
#include "../Userland/clibs/include/string.h"
#include "../Userland/clibs/include/stdio.h"
#include "../Userland/shell/include/shell.h"

#include "./include/moduleLoader.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;

DESCR_INT idt[0x100];	/* IDT de 256 entradas*/
IDTR idtr;				/* IDTR */

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	/* Load modules */
	void * moduleAddresses[] = {
		sampleCodeModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	/* Initialize kernel binary*/
	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{
	_cli();

	/* Cargar la IDT */
	setup_idt_entry(&idt[0x08], 0x08, (dword)&_timertick_handler, ACS_INT, 0);
    setup_idt_entry(&idt[0x09], 0x08, (dword)&_keyboard_handler, ACS_INT, 0);
	setup_idt_entry(&idt[0x80], 0x08, (dword)&_syscall_handler, ACS_INT, 0);
	
	/* Carga de IDTR */
	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt(&idtr);	

	/* Habilito interrupcion de timer tick*/
    _mask_pic_1(~((1 << 0) | (1 << 1) | (1 << 3) | (1 << 4)));
    _mask_pic_2(0xFF);

    /* Habilito interrupciones */
    _sti();
	_hlt();

	/* Arrancar el shell*/
    sh_init();

	return 0;
}

void
setup_idt_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}