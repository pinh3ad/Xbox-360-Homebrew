/* This header contains the trivial initialization code to set the 
\  GPU output resolution, initializte the system console, and initialize
\  USB. This code will almost never need to be modified, unless you want 
\  to specify the default resolution which is set to automatic. 
*/

#include <stdio.h>
#include <xenos/xenos.h>
#include <console/console.h>

void mainInit(){
		/* Set the console color in BGRA */
	console_set_colors(0x00000000, 0x00FF0000);	
	xenos_init(VIDEO_MODE_AUTO);
	console_init();

	usb_init();
	usb_do_poll();
	kmem_init();
}
