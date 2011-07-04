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
#include "video_init.h"
#include "Globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <dirent.h>
#include <elf/elf.h>
#include <input/input.h>
#include <xenos/xenos.h>
#include <xenon_smc/xenon_smc.h>
#include <xenon_soc/xenon_power.h>
#include <diskio/diskio.h>
#include <usb/usbmain.h>
#include <diskio/dvd.h>
#include <diskio/ata.h>

struct dirent entries[MAX_FILES];
	/* Hardware temperature values, used multiple times in code, so declared as global. */
float CPU_TMP = 0, GPU_TMP = 0, MEM_TMP = 0, MOBO_TMP = 0;

	/* Message the SMC to shut down the console. */
void shutdownConsole(){
	xenon_smc_power_shutdown();
}
	/* This function will print the system temperatures actively. */
void printTemperatures(){
	uint8_t buf[16];

        memset(buf, 0, 16);
   
        buf[0] = 0x07;
   
        xenon_smc_send_message(buf);
        xenon_smc_receive_response(buf);
   
        CPU_TMP = (float)((buf[0 * 2 + 1] | (buf[0 * 2 + 2] << 8)) / 256.0);
        GPU_TMP = (float)((buf[1 * 2 + 1] | (buf[1 * 2 + 2] << 8)) / 256.0);
        MEM_TMP = (float)((buf[2 * 2 + 1] | (buf[2 * 2 + 2] << 8)) / 256.0);
        MOBO_TMP = (float)((buf[3 * 2 + 1] | (buf[3 * 2 + 2] << 8)) / 256.0);
      
        printf("CPU = %4.2f C GPU = %4.2f C MEM = %4.2f C Mobo = %4.2f C", CPU_TMP, GPU_TMP, MEM_TMP, MOBO_TMP);
        printf("\n");
}

	/* Msg the SMC to adjust the system fan speeds. Thanks to Ced2911 :) */
void xenon_set_cpu_fan_speed(unsigned val){

	unsigned char msg[16] = { 0x94, (val & 0x7F) | 0x80 };

	xenon_smc_send_message(msg);
	
}

void xenon_set_gpu_fan_speed(unsigned val){

	unsigned char msg[16] = { 0x89, (val & 0x7F) | 0x80 };

	xenon_smc_send_message(msg);

}

	/* Attempt to automatically regulate the fan speed based on the console temperatures. */
		/* This is new, may or may not work correctly. */
void regulateTemperatures(){
	if(CPU_TMP==45){
		xenon_set_cpu_fan_speed(70);
		printf("Shifted CPU fan speed to level 1.");
	}

	if(GPU_TMP==45){
		xenon_set_gpu_fan_speed(70);
		printf("Shifted GPU fan speed to level 1.");
	}

	if(CPU_TMP==53){
		xenon_set_cpu_fan_speed(80);
		printf("Shifted CPU fan speed to level 2.");
	}

	if(GPU_TMP==53){
		xenon_set_gpu_fan_speed(80);
		printf("Shifted GPU fan speed to level 2.");
	}

	if(CPU_TMP > 58, GPU_TMP > 58){
		xenon_set_gpu_fan_speed(110);
		xenon_set_cpu_fan_speed(110);
		printf("Shifted both fans to level 3.");
	}
}

int main(){
		/* mainInit is defined in video_init.h, it handles 
		Xenos_init as well as console and USB init. */
	mainInit();
	clearScreen();
	setASCII();
	xenon_make_it_faster(XENON_SPEED_FULL);

	printf("Welcome to XDash!\n");
	printf("Version 0.03\n\n\n");

	printf("Press X to display the hardware temperatures.\n");
	printf("Press B to reboot to XeLL.\n");
	printf("Press Y to shut down the console.\n");

	printf("Press the START button at any time for main screen.\n");

	const char * s;
	char path[256];
	
		/* Handle input */
		/* This is fairly self explanitory. Assigning button presses to functions */
	struct controller_data_s controller;
	while(1){
		regulateTemperatures();
 		
		struct controller_data_s button;
 		if (get_controller_data(&button, 0))
 		{
			if((button.start)&&(!controller.start))
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
				clearScreen();
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

