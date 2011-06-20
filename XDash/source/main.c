/* XDash is an interactive console-based program to manipulate the Xbox 360 
\  While XDash is a text based menu system, it can do everything a GUI 
\  Can do. XDash can set the front panel LED color, print a 
\  live display of the hardware temperature, and shutdown the console.
\
\  XDash is still in early development but should be reletively bug free, if
\  there is a bug in the code however, feel free to notify me at:
\  kuckti15@gmail.com
\
\  Please enjoy XDash, and feel free to edit this source code as you see fit!
\  Just remember to give proper credit where credit is due.
\
\  Written by UNIX:
\  LibXenon.org
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <input/input.h>
#include <xenos/xenos.h>
#include <xenon_smc/xenon_smc.h>
#include "video_init.h"
#include "Globals.h"

	/* Message the SMC to shut down the console. */
void shutdownConsole(){
	xenon_smc_power_shutdown();
}
	/* This function will print the system temperatures actively. */
void printTemperatures(){
	uint8_t buf[16];
	float CPU_TMP = 0, GPU_TMP = 0, MEM_TMP = 0, MOBO_TMP = 0;

        memset(buf, 0, 16);
   
        buf[0] = 0x07;
   
        xenon_smc_send_message(buf);
        xenon_smc_receive_response(buf);
   
        CPU_TMP = (float)((buf[0 * 2 + 1] | (buf[0 * 2 + 2] << 8)) / 256.0);
        GPU_TMP = (float)((buf[1 * 2 + 1] | (buf[1 * 2 + 2] << 8)) / 256.0);
        MEM_TMP = (float)((buf[2 * 2 + 1] | (buf[2 * 2 + 2] << 8)) / 256.0);
        MOBO_TMP = (float)((buf[3 * 2 + 1] | (buf[3 * 2 + 2] << 8)) / 256.0);
      
        printf("CPU = %4.2f C GPU = %4.2f C MEM = %4.2f C Mobo = %4.2f C", CPU_TMP, GPU_TMP, MEM_TMP, MOBO_TMP);
        printf("\r");
}

int main(){
		/* mainInit is defined in video_init.h, it handles 
		Xenos_init as well as console and USB init. */
	mainInit();
	clearScreen();
	setASCII();

	printf("Welcome to XDash!\n");
	printf("Version 0.02\n\n\n");

	printf("Press X to display the hardware temperatures.\n");
	printf("Press B to power down the console.\n");
	printf("Press Y to set the front panel LED color.\n");

	printf("Press the BACK button at any time for main screen.\n");

		/* Handle input */
		/* This is fairly self explanitory. Assigning button presses to functions */
	struct controller_data_s controller;
	while(1){ 		
		struct controller_data_s button;
 		if (get_controller_data(&button, 0))
 		{
			if((button.select)&&(!controller.select))
			{
				clearScreen();
				reDash();
			}
			if((button.x)&&(!controller.x))
			{
				printTemperatures();
			}
			if((button.y)&&(!controller.y))
			{
				int override = rand() % 4; 
				int state = rand() % 4;
				int startanim = rand() % 4;				

				xenon_smc_set_power_led(override, state, startanim);
			}
			if((button.b)&&(!controller.b))
			{
				clearScreen();
				shutdownConsole();
			}												  
			controller=button;
		}
 		usb_do_poll();
 	}

	return 0;
}

