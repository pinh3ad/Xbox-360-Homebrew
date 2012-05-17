// Standard C includes
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Xenon includes
#include <console/console.h>
#include <xenos/xe.h>
#include <xenos/xenos.h>
#include <xenos/edram.h>
#include <libfat/fat.h>
#include <usb/usbmain.h>

#define RAND_MAX 0xFFFFFF

int main(){
	// Init console and video mode
	xenos_init(VIDEO_MODE_AUTO);
	console_init();
	usb_init();
	fatInitDefault ();
	usb_do_poll();

	printf("*! Setting up the GPU...\n");

	// Create virtual device for Xenos.
	struct XenosDevice _xe, *xe;
	xe = &_xe;

	// Initialize Xenos using the created virtual device.
	Xe_Init(xe);

	// Create and set the render target (Xenos Framebuffer).
	struct XenosSurface *fb = Xe_GetFramebufferSurface(xe);
	Xe_SetRenderTarget(xe, fb);

	// Compute a randomization seed
	srand(RAND_MAX);

	// Initialize EDRAM before the first render frame
	edram_init(xe);

	// Close the console, so it doesn't slow down rendering
	console_close();

	// Create a loop to do rendering.
	while(1){
		int RANDOM_COLOR = rand();

		// Select random color to clear to
		Xe_SetClearColor(xe, RANDOM_COLOR);

		// Resolve and clear
		Xe_Resolve(xe);

		// Synchronize frame
		Xe_Sync(xe);
	}

	return 0;
}

