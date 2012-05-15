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

// An array of bytes containing the ELF magic number
// TODO: Change this to {'E', 'L', 'F'}; ?
unsigned char ELF_MAGIC[5] = {0x7f, 0x45, 0x4c, 0x46};

// Point at a specific address to store incoming data
//unsigned char *elfAddress = (unsigned char *) 0xA00000;

int main(){
	videoInit();

	printf("!! Init finished.\n");
	printf("Press any key to enter UART binary mode.\n");

	getch();

	/* Read the first 4 bytes of incoming data. Compare them
	   to the ELF_MAGIC number, which is always the same. If
	   the first 4 bytes of data match the magic number, return
	   success, if else, return failure.

	   TODO: Instead of breaking, add continue/break so reading can
	   continue more easily.	Completed: 5-15-12

	   TODO: Execute an fread() after continue statement. Read the
	   remaining uploaded data into specific memory address. If
	   fread returns something other than 0, binary is finished uploading.
	*/
	if(kbhit()){
		// Read from UART
		unsigned char data = getch();

		while(data){

			if(sizeof(data) == 0x4) if(strcmp(data, ELF_MAGIC)) continue;
			else break;
		}
	}

	return 0;
}

