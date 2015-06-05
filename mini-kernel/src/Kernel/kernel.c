#include <stdint.h>

#include "./include/kasm.h"
#include "./include/defs.h"
#include "./include/keyboard.h"
#include "./include/syscall.h"

#include "./include/libc.h"

#include "./include/moduleLoader.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const shell_module_address = (void*)0x400000;
static void * const libc_module_address = (void*)0x500000;
static void * const syscall_module_address = (void*)0x600000;

IDTR idtr; /* IDTR description*/

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
		shell_module_address,
		libc_module_address,
		syscall_module_address
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	/* Initialize kernel binary*/
	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
	{
	_cli();

	_get_idtr(&idtr);

	/* setear los handlers en la IDT */
	_set_idt_entry(0x20, &_pit_handler, &(idtr.base));
  _set_idt_entry(0x21, &_keyboard_handler, &(idtr.base));
	_set_idt_entry(0x80, &_syscall_handler, &(idtr.base));

	/* Habilito interrupcion de teclado*/
	_mask_pic();

	/* Habilito interrupciones */
	_sti();
	_hlt();

	return 0;
}