// Get binary data over serial, store it in memory, execute it from memory.
// Written by UNIX - LibXenon.org

// TODO: Test.

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "video_init.h"

#include <xenon_uart/xenon_uart.h>
#include <elf/elf.h>
#include <time/time.h>

// An array of bytes containing the ELF magic number.
unsigned char ELF_MAGIC[4] = {'E', 'L', 'F'};

int main(){
	videoInit();
	console_clrscr();

	printf("Press any key to enter UART binary mode.\n");

	// Instantiate a memory address where we will setup binary data in.
	unsigned char *elfAddress = malloc(0x1000000);

	while(1){
		// Variable to hold read binary data.
		unsigned char data = getch();

		// Instantiate a counter for the number of bytes we have read.
		long int bytesRead = 0;

		// Print the contents of memory buffer to console
		printf("%x\n", &elfAddress);

		// Up the counter before we read the next byte.
		bytesRead++;
	}

	return 0;
}

