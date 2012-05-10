// Standard C includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Local includes
#include "video_init.h"

// Xenon includes
#include <elf/elf.h>

/* Create a void pointer for the executeable at a specific address
   so we can refernce it later.
*/
void *elfPosition = (void *)0x1400000;
int elfSize;

int main(){
	// Init console and video
	videoInit();

	// Create FILE type for our elf to load from USB to memory address
	FILE *elfFile = fopen("uda:/xenon.elf32", "rb");

	// Get the size of the elf
	fseek(elfFile, 0, SEEK_END);
	elfSize = ftell(elfFile);
	rewind(elfFile);

	/* Now that we have all the info we need about the file,
	   we can read the executeable into the memory address
	   we defined at the top of the file.
	*/
	fread(&elfPosition[elfSize], 1, elfSize, elfFile);

	// Execute the elf from the memory address
	elf_runFromMemory(elfPosition, elfSize);
}

