/* This header contains the trivial initialization code to set the 
\  GPU output resolution, initializte the system console, and initialize
\  USB. This code will almost never need to be modified, unless you want 
\  to specify the default resolution which is set to automatic. 
*/

#include <stdio.h>
#include <xenos/xenos.h>
#include <console/console.h>

void videoInit(){
	xenos_init(VIDEO_MODE_VGA_1280x768);
	console_init();
}
