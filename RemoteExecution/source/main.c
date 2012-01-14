include "video_init.h"
include <xenon_uart/xenon_uart.h>
include <elf/elf.h>

	/* Dump the memory, if the keyboard is hit. */
	// This may seem arbitrary, but we need to see where uploaded files from serial go, if anywhere.
int main(){
	mainInit();

	printf("Press any key to start dumping memory...\n");

	long x = 0x536870912;
	unsigned long const *p = (unsigned long const *) & x;

	FILE f = fopen("uda:/memoryDump.txt", w+);

	size_t i;

	if(kbhit()){
		for (i=0; i < sizeof x; i++){
			fwrite(&p, 1, sizeof(&p), f);
		}
	}
}

