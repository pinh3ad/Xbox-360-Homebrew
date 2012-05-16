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
unsigned char ELF_MAGIC[4] = {0x7f, 0x45, 0x4c, 0x46};

// Point at a specific address to store incoming data
unsigned char *elfAddress = (unsigned char *) 0xA00000;

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
		// Create FILE type for incoming data. We will use this later.
		FILE * elfFile = fopen(data, "rb");

		while(data){
			/* If size of "data" is 4 bytes, cross check those 4 bytes
			   with the ELF_MAGIC number array to see if what we are
			   reading is an ELF. If yes, continue loop, if else, break.
			*/
			if(sizeof(data) == 0x4){
				if(strcmp(data, ELF_MAGIC));
					continue;
			}
			else{
				break;
			}

			/* If we pass the condition of the if above, execute fread()
			   on data, to our memory pointer we are storing the ELF to.

			   elfAddress: Specific memory address (0xA00000) to store ELF.
			   dataSize: Size of each data element (0x1)
			   dataElements: Number of elements to read (Needs moar magic)
			   elfFile: FILE type set to "data", the actual binary data to be stored
			*/
			int dataSize = 0x1;

			/* TODO: Need to add magic here, if I remember there is and ELF section
			   for file size.
			*/
			int dataElements = sizeof(data);

			fread(elfAddress, dataSize, dataElements, elfFile);
		}
	}

	return 0;
}

