char * v = (char*)0xB8000 + 79 * 2;

// this is read from the ld
extern char bss;
extern int endOfBinary;

int main() {
	*v = 'X';
	&bss;
	&endOfBinary;
	return 0xDEADBEEF;
}
