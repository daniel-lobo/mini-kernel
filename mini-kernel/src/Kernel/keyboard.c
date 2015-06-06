#include "./include/keyboard.h"
#include "./include/kasm.h"
#include <stdint.h>

/* El Buffer de Teclado guarda scan codes */
unsigned char keyboard_buffer[KB_SIZE];

/* Indices para iterar sobre el buffer ciclico */
int kb_actual = 0;
int kb_last   = 0;
int kb_cant   = 0;

/* Estados para estados de shift, control, capslock estan en cero */
int acute    = 0;
int ctrl     = 0;
int shift    = 0;
int alt      = 0;
int capslock = 0;

/* Variable que guarda la ultima tecla leida */
unsigned char last_key_read = 0;

// English keyboard: with the letters corresponding to shift
static unsigned int kbd_EN[][2] = { { NPRTBL, NPRTBL },/*000*/
																    { NPRTBL, NPRTBL },/*001 ESCAPE*/
																    { '1', '!' }, /*002*/
																    { '2', '\"' }, /*003*/
																    { '3', '#' }, /*004*/
																    { '4', '$' }, /*005*/
																    { '5', '%' }, /*006*/
																    { '6', '&' }, /*007*/
																    { '7', '/' }, /*008*/
																    { '8', '(' }, /*009*/
																    { '9', ')' }, /*010*/
																    { '0', '=' }, /*011*/
																    { '\'', '?' }, /*012*/
																    { '\n', '\n' }, /*013*/
																    { '\b', '\b' }, /*014 BACKSPACE*/
																    { '\t', '\t' }, /*015 TAB*/
																    { 'q', 'Q' }, /*016*/
																    { 'w', 'W' }, /*017*/
																    { 'e', 'E' }, /*018*/
																    { 'r', 'R' }, /*019*/
																    { 't', 'T' }, /*020*/
																    { 'y', 'Y' }, /*021*/
																    { 'u', 'U' }, /*022*/
																    { 'i', 'I' }, /*023*/
																    { 'o', 'O' }, /*024*/
																    { 'p', 'P' }, /*025*/
																    { '\'', '\"' }, /*026*/
																    { '+', '*' }, /*027*/
																    { '\n', '\n' }, /*028*/
																    { NPRTBL, NPRTBL },/*029 CTRL IZQ*/
																    { 'a', 'A' }, /*030*/
																    { 's', 'S' }, /*031*/
																    { 'd', 'D' }, /*032*/
																    { 'f', 'F' }, /*033*/
																    { 'g', 'G' }, /*034*/
																    { 'h', 'H' }, /*035*/
																    { 'j', 'J' }, /*036*/
																    { 'k', 'K' }, /*037*/
																    { 'l', 'L' }, /*038*/
																    { '.', ';' }, /*039*/
																    { '{', '[' }, /*040*/
																    { '}', ']' }, /*041*/
																    { NPRTBL, NPRTBL },/*042 SHIFT IZQ*/
																    { '<', '>' }, /*043*/
																    { 'z', 'Z' }, /*044*/
																    { 'x', 'X' }, /*045*/
																    { 'c', 'C' }, /*046*/
																    { 'v', 'V' }, /*047*/
																    { 'b', 'B' }, /*048*/
																    { 'n', 'N' }, /*049*/
																    { 'm', 'M' }, /*050*/
																    { ',', ';' }, /*051*/
																    { '.', ':' }, /*052*/
																    { '?', '?' }, /*053*/
																    { NPRTBL, NPRTBL },/*054 SHIFT DER*/
																    { '*', '*' }, /*055 KEY **/
																    { NPRTBL, NPRTBL },/*056 ALT IZQ*/
																    { ' ', ' ' }, /*057 SPACE*/
																    { NPRTBL, NPRTBL }, /*058 CAPSLOCK*/
																    { NPRTBL, NPRTBL }, /*059 F1*/
																    { NPRTBL, NPRTBL }, /*060 F2*/
																    { NPRTBL, NPRTBL }, /*061 F3*/
																    { NPRTBL, NPRTBL }, /*062 F4*/
																    { NPRTBL, NPRTBL }, /*063 F5*/
																    { NPRTBL, NPRTBL }, /*064 F6*/
																    { NPRTBL, NPRTBL }, /*065 F7*/
																    { NPRTBL, NPRTBL }, /*066 F8*/
																    { NPRTBL, NPRTBL }, /*067 F9*/
																    { NPRTBL, NPRTBL }, /*068 F10*/
																    { NPRTBL, NPRTBL }, /*069 NUM LOCK*/
																    { NPRTBL, NPRTBL }, /*070 SCROLL LOCK*/
																    { '7', '7' }, /*071 KEY 7*/
																    { '8', '8' }, /*072 KEY 8*/
																    { '9', '9' }, /*073 KEY 9*/
																    { '-', '-' }, /*074 KEY -*/
																    { '4', '4' }, /*075 KEY 4*/
																    { '5', '5' }, /*076 KEY 5*/
																    { '6', '6' }, /*077 KEY 6*/
																    { '+', '+' }, /*078 KEY +*/
																    { '1', '1' }, /*079 KEY 1*/
																    { '2', '2' }, /*080 KEY 2*/
																    { '3', '3' }, /*081 KEY 3*/
																    { '0', '0' }, /*082 KEY 0*/
																    { '.', '.' }, /*083 KEY .*/
																    { NPRTBL, NPRTBL },/*084 SYS REQ (AT)*/
																    { '+', '*' }, /*085*/
																    { '+', '*' }, /*086*/
																    { NPRTBL, NPRTBL },/*087 F11*/
																    { NPRTBL, NPRTBL },/*088 F12*/
																    { '+', '*' }, /*089*/
																    { '+', '*' } /*090*/
};

// // Spanish keyboard: with the letters corresponding to shift
// // static unsigned int kbd_ES[][2] = { { NPRTBL, NPRTBL },/*000*/
// // 																    { NPRTBL, NPRTBL },/*001 ESCAPE*/
// // 																    { '1', '!' }, /*002*/
// // 																    { '2', '"' }, /*003*/
// // 																    { '3', '#' }, /*004*/
// // 																    { '4', '$' }, /*005*/
// // 																    { '5', '%' }, /*006*/
// // 																    { '6', '&' }, /*007*/
// // 																    { '7', '/' }, /*008*/
// // 																    { '8', '(' }, /*009*/
// // 																    { '9', ')' }, /*010*/
// // 																    { '0', '=' }, /*011*/
// // 																    { '\'', '?' }, /*012*/
// // 																    { '\n', '\n' }, /*013*/
// // 																    { '\b', '\b' }, /*014 BACKSPACE*/
// // 																    { '\t', '\t' }, /*015 TAB*/
// // 																    { 'q', 'Q' }, /*016*/
// // 																    { 'w', 'W' }, /*017*/
// // 																    { 'e', 'E' }, /*018*/
// // 																    { 'r', 'R' }, /*019*/
// // 																    { 't', 'T' }, /*020*/
// // 																    { 'y', 'Y' }, /*021*/
// // 																    { 'u', 'U' }, /*022*/
// // 																    { 'i', 'I' }, /*023*/
// // 																    { 'o', 'O' }, /*024*/
// // 																    { 'p', 'P' }, /*025*/
// // 																    { '\'', '\"' }, /*026*/
// // 																    { '+', '*' }, /*027*/
// // 																    { '\n', '\n' }, /*028*/
// // 																    { NPRTBL, NPRTBL },/*029 CTRL IZQ*/
// // 																    { 'a', 'A' }, /*030*/
// // 																    { 's', 'S' }, /*031*/
// // 																    { 'd', 'D' }, /*032*/
// // 																    { 'f', 'F' }, /*033*/
// // 																    { 'g', 'G' }, /*034*/
// // 																    { 'h', 'H' }, /*035*/
// // 																    { 'j', 'J' }, /*036*/
// // 																    { 'k', 'K' }, /*037*/
// // 																    { 'l', 'L' }, /*038*/
// // 																    { 164, 165 }, /*039*/
// // 																    { '{', '[' }, /*040*/
// // 																    { '}', ']' }, /*041*/
// // 																    { NPRTBL, NPRTBL },/*042 SHIFT IZQ*/
// // 																    { '<', '>' }, /*043*/
// // 																    { 'z', 'Z' }, /*044*/
// // 																    { 'x', 'X' }, /*045*/
// // 																    { 'c', 'C' }, /*046*/
// // 																    { 'v', 'V' }, 047
// // 																    { 'b', 'B' }, /*048*/
// // 																    { 'n', 'N' }, /*049*/
// // 																    { 'm', 'M' }, /*050*/
// // 																    { ',', ';' }, /*051*/
// // 																    { '.', ':' }, /*052*/
// // 																    { '-', '_' }, /*053*/
// // 																    { NPRTBL, NPRTBL },/*054 SHIFT DER*/
// // 																    { '*', '*' }, /*055 KEY **/
// // 																    { NPRTBL, NPRTBL },/*056 ALT IZQ*/
// // 																    { ' ', ' ' }, /*057 SPACE*/
// // 																    { NPRTBL, NPRTBL },/*058 CAPSLOCK*/
// // 																    { NPRTBL, NPRTBL },/*059 F1*/
// // 																    { NPRTBL, NPRTBL },/*060 F2*/
// // 																    { NPRTBL, NPRTBL },/*061 F3*/
// // 																    { NPRTBL, NPRTBL },/*062 F4*/
// // 																    { NPRTBL, NPRTBL },/*063 F5*/
// // 																    { NPRTBL, NPRTBL },/*064 F6*/
// // 																    { NPRTBL, NPRTBL },/*065 F7*/
// // 																    { NPRTBL, NPRTBL },/*066 F8*/
// // 																    { NPRTBL, NPRTBL },/*067 F9*/
// // 																    { NPRTBL, NPRTBL },/*068 F10*/
// // 																    { NPRTBL, NPRTBL },/*069 NUM LOCK*/
// // 																    { NPRTBL, NPRTBL },/*070 SCROLL LOCK*/
// // 																    { '7', '7' }, /*071 KEY 7*/
// // 																    { '8', '8' }, /*072 KEY 8*/
// // 																    { '9', '9' }, /*073 KEY 9*/
// // 																    { '-', '-' }, /*074 KEY -*/
// // 																    { '4', '4' }, /*075 KEY 4*/
// // 																    { '5', '5' }, /*076 KEY 5*/
// // 																    { '6', '6' }, /*077 KEY 6*/
// // 																    { '+', '+' }, /*078 KEY +*/
// // 																    { '1', '1' }, /*079 KEY 1*/
// // 																    { '2', '2' }, /*080 KEY 2*/
// // 																    { '3', '3' }, /*081 KEY 3*/
// // 																    { '0', '0' }, /*082 KEY 0*/
// // 																    { '.', '.' }, /*083 KEY .*/
// // 																    { NPRTBL, NPRTBL },/*084 SYS REQ (AT)*/
// // 																    { '+', '*' }, /*085*/
// // 																    { '+', '*' }, /*086*/
// // 																    { NPRTBL, NPRTBL },/*087 F11*/
// // 																    { NPRTBL, NPRTBL },/*088 F12*/
// // 																    { '+', '*' }, /*089*/
// // 																    { '+', '*' } /*090*/
// // };

// void
// sendToBuffer(char scancode)
// {
// 	// if(!k_buffer_is_full())
// 	// {
//  //    keyboard_buffer[kb_actual++] = scancode;
//  //    kb_cant++;

//  //    // to avoid buffer overflow, it starts again, writing over the stuff...
//  //    if(kb_actual == KB_SIZE){
//  //      kb_actual = 0;
//  //    }
// 	// }

// 	// set_key_state(scancode); //Actualiza los estados del shift
// 	// return;
// }


/* lee del buffer de teclado
 * devuelve el caracter o EOF
*/
// int
// getKchar(void)
// {
// 	// unsigned char c;

// 	// if(!k_buffer_is_empty())
// 	// {
//  //    c = scancodeToChar(keyboard_buffer[kb_last++]);
//  //    kb_cant--;
    
//  //    // to avoid buffer overflow, it starts again, writing over the stuff...
//  //    if(kb_last == KB_SIZE){
//  //      kb_last = 0;
//  //    }

//  //    return c;
// 	// }
// 	// return NPRTBL;
// }


static int
k_buffer_is_full(void)
{
	return kb_cant == KB_SIZE ? 1 : 0;
}

int
k_buffer_is_empty(void)
{
	return (kb_cant == 0) ? 1 : 0;
}

void
set_key_state(uint64_t scancode)
{
	int position = (unsigned int) scancode;

	switch(scancode){
		case (SCANCODE_CHAR_RALT):
			alt = !alt;
			break;
		case  (SCANCODE_CHAR_LALT):
			alt = !alt;
			break;
		case (SCANCODE_CHAR_RSHIFT):
			shift = !shift;
			break;
		case (SCANCODE_CHAR_LSHIFT):
			shift = !shift;
			break;
		case (SCANCODE_CHAR_LCTRL):
			ctrl = !ctrl;
			break;
		case (SCANCODE_CHAR_RCTRL):
			ctrl = !ctrl;
			break;
		default:
			return_character_position(position);
			break;
	}
	return;
}

char
return_character_position(int position){
	if (kbd_shifted())
	{
		char keyboard_symbol = kbd_EN[position][1];
	} else if(kbd_capitalized()){
		char keyboard_symbol = kbd_EN[position][1];
	} else if(kbd_alted()){
		char keyboard_symbol = kbd_EN[position][2];
	}	else {
		char keyboard_symbol = kbd_EN[position][0];
	}
}
void keyboard_handler(uint64_t scancode){
	set_key_state(scancode);

}

int 
kbd_shifted(void)
{
    return shift;
}

int
kbd_alted(void)
{
	return alt;
}

int 
kbd_capitalized(void)
{
    return ((shift) && !capslock) || (!(shift) && capslock);
}




