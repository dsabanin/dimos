#include "drivers/screen.h"

size_t mystrlen(char* str) {
	size_t len = 0;
	char* cur = str;
	while(*cur != '\0') {
		len++;
		cur++;
	}
	return len;
}

void main() {
	clear_screen();
	kprint("Arrived in C kernel code\n");
	kprint("VELKAM!\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("6\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("1\n");
	kprint("5\n");
	kprint("1\n\n\n");
}
