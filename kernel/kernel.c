#include "../drivers/screen.h"

int mystrlen(char* str) {
	unsigned int len = 0;
	char* cur = str;
	while(*cur != '\0') {
		len++;
		cur++;
	}
	return len;
}

void  main() {
	clear_screen();
	kprint("Arrived in C kernel code\n");
	kprint("VELKAM!\n");
}
