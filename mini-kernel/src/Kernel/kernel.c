#include <stdint.h>

#include "./include/kasm.h"
#include "./include/defs.h"
#include "./include/keyboard.h"
#include "./include/syscall.h"
#include "./include/video.h"
#include "./include/rtc.h"
#include "./include/mmu.h"

#include "./include/libc.h"

#include "./include/moduleLoader.h"

#include "./include/naiveConsole.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000; //8kb

static void * const shell_module_address = (void*)0x800000;
static void * const data_module_address = (void*)0x900000;

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
		+ PageSize * 1536				//The size of the stack itself, 6MB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{

	/* Load modules */
	// ncPrint("[Loading modules]");
	// ncNewline();
	void * moduleAddresses[] = {
		shell_module_address,
		data_module_address
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	// ncPrint("[Done]");
	// ncNewline();
	// ncPrint("[Initializing kernel's binary]");
	// ncNewline();

	/* Initialize kernel binary*/
	clearBSS(&bss, &endOfKernel - &bss);

	// ncPrint("  text: 0x");
	// ncPrintHex((uint64_t)&text);
	// ncNewline();
	// ncPrint("  rodata: 0x");
	// ncPrintHex((uint64_t)&rodata);
	// ncNewline();
	// ncPrint("  data: 0x");
	// ncPrintHex((uint64_t)&data);
	// ncNewline();
	// ncPrint("  bss: 0x");
	// ncPrintHex((uint64_t)&bss);
	// ncNewline();
	// ncPrint("  end of binary: 0x");
	// ncPrintHex((uint64_t)&endOfKernel);
	// ncNewline();
	// ncPrint("  stack base: 0x");
	// ncPrintHex((uint64_t)getStackBase());
	// ncNewline();
	// ncPrint("[Done]");
	// ncNewline();

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

	/*fix rtc*/
	rtc_fix();

	// Start heap from binary end
	init_heap(&endOfKernel);

	((EntryPoint)shell_module_address)();

	// ncPrint("[Kernel Main]");
	// ncNewline();
	// ncPrint("  Sample code module at 0x");
	// ncPrintHex((uint64_t)shell_module_address);
	// ncNewline();
	// ncPrint("  Calling the sample code module returned: ");
	// ncPrintHex(((EntryPoint)shell_module_address)());
	// ncNewline();
	// ncNewline();

	// ncPrint("  Sample data module at 0x");
	// ncPrintHex((uint64_t)data_module_address);
	// ncNewline();
	// ncPrint("  Sample data module contents: ");
	// ncPrint((char*)data_module_address);
	// ncNewline();

	// ncPrint("[Finished]");	
	
	return 0;
}