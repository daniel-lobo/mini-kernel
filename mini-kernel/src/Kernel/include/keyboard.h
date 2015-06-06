#include <stdint.h>

#ifndef _KB_
#define _KB_

#define NPRTBL 0
#define KB_SIZE 2000

// ALT
#define SCANCODE_EXT 0x80
#define SCANCODE_KEY_62 (SCANCODE_EXT | 0x38)
#define SCANCODE_CHAR_RALT SCANCODE_KEY_62

#define SCANCODE_KEY_60 0x38
#define SCANCODE_CHAR_LALT SCANCODE_KEY_60

// SHIFT
#define SCANCODE_KEY_57 0x36
#define SCANCODE_CHAR_RSHIFT SCANCODE_KEY_57

#define SCANCODE_KEY_44 0x2a
#define SCANCODE_CHAR_LSHIFT SCANCODE_KEY_44

// CTRL
#define SCANCODE_KEY_64 (SCANCODE_EXT | 0x1d)
#define SCANCODE_CHAR_RCTRL SCANCODE_KEY_64

#define SCANCODE_KEY_58 0x1d
#define SCANCODE_CHAR_LCTRL SCANCODE_KEY_58

// ARROWS
#define SCANCODE_KEY_79 (SCANCODE_EXT | 0x4b)
#define SCANCODE_CHAR_LARROW SCANCODE_KEY_79

#define SCANCODE_KEY_83 (SCANCODE_EXT | 0x48)
#define SCANCODE_CHAR_UPARROW SCANCODE_KEY_83

#define SCANCODE_KEY_84 (SCANCODE_EXT | 0x50)
#define SCANCODE_CHAR_DNARROW SCANCODE_KEY_84

#define SCANCODE_KEY_89 (SCANCODE_EXT | 0x4d)
#define SCANCODE_CHAR_RARROW SCANCODE_KEY_89

// ENTER
#define SCANCODE_KEY_43 0x1c
#define SCANCODE_CHAR_ENTER SCANCODE_KEY_43

// SPACE
#define SCANCODE_KEY_61 0x39
#define SCANCODE_CHAR_SPACE SCANCODE_KEY_61

// TAB
#define SCANCODE_KEY_16 0xf
#define SCANCODE_CHAR_TAB SCANCODE_KEY_16

// CAPSLOCK
#define SCANCODE_KEY_30 0x3a
#define SCANCODE_CHAR_CAPSLOCK SCANCODE_KEY_30

// BACKSPACE
#define SCANCODE_KEY_15 0xe
#define SCANCODE_CHAR_BACKSPACE SCANCODE_KEY_15


void keyboard_handler();
// Returns if the buffer is full or not
static int k_buffer_is_full(void);
// para el manejo de las teclas de estado (SHIFT y CAPSLOCK)
void set_key_state(uint64_t scancode);

void sendToBuffer(char scancode);
int getKchar(void);
int k_buffer_is_empty(void);
char return_character_position(void);
int kbd_shifted(void);
int kbd_alted(void);
int kbd_capitalized(void);
#endif

