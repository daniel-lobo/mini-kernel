#include <stdint.h>

#include "./include/kasm.h"
#include "./include/defs.h"
#include "./include/keyboard.h"
#include "./include/syscall.h"
#include "./include/video.h"
#include "./include/rtc.h"

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
static void * const data_module_address = (void*)0x500000;

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
		data_module_address
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	/* Initialize kernel binary*/
	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{
	/* IDT & handlers setup */
	_set_handlers();
	_enable_pic();

	
	/* Inicializar driver de video */
	video_init();	 
	video_init_screensaver();

	/* buffer de teclado*/
	initializeBuffer();

	((EntryPoint)shell_module_address)();
	
	return 0;
}