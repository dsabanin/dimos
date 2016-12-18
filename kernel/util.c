#include "kernel/util.h"

void memory_copy(char* src, char* dest, size_t bytes) {
	for(size_t i=0; i<bytes; i++) {
		*(dest + i) = *(src + i);
	}
}
