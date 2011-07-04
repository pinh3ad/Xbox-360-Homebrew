/* This program will allow for manipulation of the front LED colors :)
\  Mostly for fun project :D
*/

#include <stdio.h>
#include <console/console.h>
#include <input/input.h>
#include <xenos/xenos.h>
#include <xenos/xe.h>
#include <xenos_smc/xenon_smc.h>
#include "video_init.h"

int main(){
	mainInit();

	xenon_smc_set_power_led(4, 4, 0);

	printf("Press B to reboot.");
	printf("Press A to change LED colors.");

	struct controller_data_s controller;
	while(1){
		struct controller_data_s button;
		if (get_controller_data(&button, 0)){
			if((button.b)&&(!controller.b))
			{
				xenon_smc_power_reboot();
			}
		}
	}

	return 0;
}

