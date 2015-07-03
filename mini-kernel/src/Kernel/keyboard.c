#include "./include/keyboard.h"
#include "./include/kasm.h"
#include "./include/video.h"
#include <stdint.h>

KEYBOARD currentKeyboard;
// English keyboard: with the letters corresponding to shift
int stroke_counter = 0;

char kbd_EN[][4] = {
	//USA
	{ 0x00, NOCHAR, NOCHAR, NOCHAR }, //empty,
	{ 0x01, NOCHAR, NOCHAR, NOCHAR }, //esc
	{ 0x02, '1', '!', '1' },
	{ 0x03, '2', '@', '2' },
	{ 0x04, '3', '#', '3' },
	{ 0x05, '4', '$', '4' },
	{ 0x06, '5', '%', '5' },
	{ 0x07, '6', '^', '6' },
	{ 0x08, '7', '&', '7' },
	{ 0x09, '8', '*', '8' },
	{ 0x0A, '9', '(', '9' },
	{ 0x0B, '0', '"', '0' },
	{ 0x0C, '-', '_', '-' },
	{ 0x0D, '=', '+', NOCHAR },
	{ 0x0E, '\b', '\b', '\b' }, //backspace
	{ 0x0F, '\t', '\t', '\t' }, //tab
	{ 0x10, 'q', 'Q', 'Q' },
	{ 0x11, 'w', 'W', 'W' },
	{ 0x12, 'e', 'E', 'E' },
	{ 0x13, 'r', 'R', 'R' },
	{ 0x14, 't', 'T', 'T' },
	{ 0x15, 'y', 'Y', 'Y' },
	{ 0x16, 'u', 'U', 'U' },
	{ 0x17, 'i', 'I', 'I' },
	{ 0x18, 'o', 'O', 'O' },
	{ 0x19, 'p', 'P', 'P' },
	{ 0x1a, '[', '{', '[' },
	{ 0x1b, ']', '}', ']' },
	{ 0x1c, '\n', '\n', '\n' },//enter
	{ 0x1d, NOCHAR, NOCHAR, NOCHAR },//left ctrl
	{ 0x1e, 'a', 'A', 'A' },
	{ 0x1f, 's', 'S', 'S' },
	{ 0x20, 'd', 'D', 'D' },
	{ 0x21, 'f', 'F', 'F' },
	{ 0x22, 'g', 'G', 'G' },
	{ 0x23, 'h', 'H', 'H' },
	{ 0x24, 'j', 'J', 'J' },
	{ 0x25, 'k', 'K', 'K' },
	{ 0x26, 'l', 'L', 'L' },
	{ 0x27, ';', ':', ';' },
	{ 0x28, '\'', '"', '\'' },
	{ 0x29, '`', '~', '`' },
	{ 0x2a, LEFT_SHIFT_MAKE, LEFT_SHIFT_MAKE, LEFT_SHIFT_MAKE },//left shift
	{ 0x2b, '\\', '|', NOCHAR },
	{ 0x2c, 'z', 'Z', 'Z' },
	{ 0x2d, 'x', 'X', 'X' },
	{ 0x2e, 'c', 'C', 'C' },
	{ 0x2f, 'v', 'V', 'V' },
	{ 0x30, 'b', 'B', 'B' },
	{ 0x31, 'n', 'N', 'N' },
	{ 0x32, 'm', 'M', 'M' },
	{ 0x33, ',', '<', ',' },
	{ 0x34, '.', '>', '.' },
	{ 0x35, '/', '?', '/' },
	{ 0x36, RIGHT_SHIFT_MAKE, RIGHT_SHIFT_MAKE, RIGHT_SHIFT_MAKE },//right shift
	{ 0x37, '*', NOCHAR, NOCHAR  },//keypad *
	{ 0x38, NOCHAR, NOCHAR, NOCHAR  },//left alt
	{ 0x39, ' ', ' ', NOCHAR  },
	{ 0x3a, CAPS_LOCK, CAPS_LOCK, CAPS_LOCK  },//caps
	{ 0x3b, NOCHAR, NOCHAR, NOCHAR  },//f1
	{ 0x3c, NOCHAR, NOCHAR, NOCHAR  },//f2
	{ 0x3d, NOCHAR, NOCHAR, NOCHAR  },//f3
	{ 0x3e, NOCHAR, NOCHAR, NOCHAR  },//f4
	{ 0x3f, NOCHAR, NOCHAR, NOCHAR  },//f5
	{ 0x40, NOCHAR, NOCHAR, NOCHAR  },//f6
	{ 0x41, NOCHAR, NOCHAR, NOCHAR  },//f7
	{ 0x42, NOCHAR, NOCHAR, NOCHAR  },//f8
	{ 0x43, NOCHAR, NOCHAR, NOCHAR  },//f9
	{ 0x44, NOCHAR, NOCHAR, NOCHAR  },//f10
	{ 0x45, NOCHAR, NOCHAR, NOCHAR  },//num
	{ 0x46, NOCHAR, NOCHAR, NOCHAR  },//scroll
	{ 0x47, '7', NOCHAR, NOCHAR  },//keypad 7
	{ 0x48, NOCHAR, NOCHAR, NOCHAR  },//keypad 8
	{ 0x49, '9', NOCHAR, NOCHAR  },//keypad 9
	{ 0x4a, '-', NOCHAR, NOCHAR  },//keypad -
	{ 0x4b, NOCHAR, NOCHAR, NOCHAR  },//keypad 4
	{ 0x4c, '5', NOCHAR, NOCHAR  },//keypad 5
	{ 0x4d, NOCHAR, NOCHAR, NOCHAR  },//keypad 6
	{ 0x4e, '+', NOCHAR, NOCHAR  },//keypad +
	{ 0x4f, '1', NOCHAR, NOCHAR  },//keypad 1
	{ 0x50, NOCHAR, NOCHAR, NOCHAR  },//keypad 2
	{ 0x51, '3', NOCHAR, NOCHAR  },//keypad 3
	{ 0x52, '0', NOCHAR, NOCHAR  },//keypad 0
	{ 0x53, '.', NOCHAR, NOCHAR  },//keypad 0
	{ 0x57, NOCHAR, NOCHAR, NOCHAR },//f11
	{ 0x58, NOCHAR, NOCHAR, NOCHAR }//f12
};

bool bufferIsEmpty(void) {
	return currentKeyboard.enqueuePos == currentKeyboard.dequeuePos;
}

bool bufferIsFull(void) {
	return currentKeyboard.enqueuePos == BUFFER_SIZE;
}

void initializeBuffer() {
	int i;
	currentKeyboard.enqueuePos = currentKeyboard.dequeuePos = 0;

	for (i = 0; i < BUFFER_SIZE; i++) {
		currentKeyboard.buffer[i] = 0;
	}
	currentKeyboard.state.shifted = 0;
	currentKeyboard.state.alted = 0;
	currentKeyboard.state.capsLocked = 0;
}

bool sendToBuffer(char key) {
	if (bufferIsFull() == true) {
		currentKeyboard.enqueuePos = 0;
		currentKeyboard.buffer[0] = (unsigned char)key;
		currentKeyboard.enqueuePos++;
		return true;
	}
	currentKeyboard.buffer[currentKeyboard.enqueuePos] = (unsigned char)key;
	currentKeyboard.enqueuePos++;
	return true;
}

bool updateStates(char key) {
	if (key == LEFT_SHIFT_MAKE) {
		currentKeyboard.state.shifted = !currentKeyboard.state.shifted;
		return true;
	}
	else if (key == CAPS_LOCK) {
		if (currentKeyboard.state.capsLocked ==0){
			currentKeyboard.state.capsLocked =1;
		} else {
			currentKeyboard.state.capsLocked =0;
		}
		return true;
	}
	return false;
}

int indexOfKey() {
	if (currentKeyboard.state.shifted == 1) {
		return SHIFTEDCOLUMN;
	}
	else if (currentKeyboard.state.capsLocked == 1) {
		return CAPSLOCKEDCOLUMN;
	}
	return NORMALCOLUMN;
}

void keyboard_handler(uint64_t scancode) {
	int index= indexOfKey();
	char key = kbd_EN[(int)scancode][index];

	if(scancode & 0x80){
		if(scancode == LEFT_SHIFT_BREAK || scancode == RIGHT_SHIFT_BREAK) {
			currentKeyboard.state.shifted = 0;	
		}
		return;	
	}
	if (updateStates(key) == true) {
		return;
	}
	else {
		if(key != NOCHAR){
			if (!is_ss_on()){
				sendToBuffer(key);
			}
		}	
	}
	video_refresh();
	return;
}

int
pop()
{
	int c = -1;
	if (!bufferIsEmpty())
	{
		c = currentKeyboard.buffer[currentKeyboard.dequeuePos];
		currentKeyboard.dequeuePos = (currentKeyboard.dequeuePos+1) % BUFFER_SIZE;
	}
	return c;
}

void
delete_from_buffer()
{
	if (!bufferIsEmpty())
	{
		int i = currentKeyboard.dequeuePos;
		for (i = 0; i < BUFFER_SIZE - 1; i++){
			currentKeyboard.buffer[i] = currentKeyboard.buffer[i + 1];
		}
	}
}

void
clean_buffer()
{
	int i = 0;
	for (i = 0; i < BUFFER_SIZE; i++)
	{
		currentKeyboard.buffer[i] = 0;
	}
	currentKeyboard.enqueuePos = 0;
	currentKeyboard.dequeuePos = 0;
}