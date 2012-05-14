// Get binary data over serial, store it in memory, execute it from memory.
// Written by UNIX - LibXenon.org

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "video_init.h"

#include <xenon_uart/xenon_uart.h>
#include <elf/elf.h>

// An array of bytes containing the ELF magic number
unsigned char ELF_MAGIC[4] = {'0x7f', '0x45', '0x4c', '0x46' };

// Point at a specific memory address
unsigned char *address = (unsigned char *) 0xA00000;

int main(){
	videoInit();

	// Set i equal to 0 bytes
	int i = 0x0;

	/* If UART status is okay, execute a loop. Within that loop,
	   getch(), and 
	*/
	if(kbhit()){
		unsigned char data = getch();

		while(data){
			i++;

			if(i == 0x4){
				if(data == ELF_MAGIC){
					break;
				}
			}
		}
	}

	printf("Success status 0!\n");

	return 0;
}

