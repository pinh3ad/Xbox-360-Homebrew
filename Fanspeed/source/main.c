/* This program will adjust the fan speed on the Xbox to near 100%
\  by messaging the SMC. It isn't permanent, so when the app is not
\  running your fan speed will be normal. 
\
\  Thanks to Ced2911 for the functions. 
\
\  *I plan to incorporate a setting for this in XDash.
*/

#include <stdio.h>
#include <xenon_smc/xenon_smc.h>
#include "video_init.h"

void xenon_set_cpu_fan_speed(unsigned val){

unsigned char msg[16] = { 0x94, (val & 0x7F) | 0x80 };

xenon_smc_send_message(msg);

}

void xenon_set_gpu_fan_speed(unsigned val){

unsigned char msg[16] = { 0x89, (val & 0x7F) | 0x80 };

xenon_smc_send_message(msg);

}

int main(){
	mainInit();

	printf("Setting fan speed to 100 (int)");

	while(1){
		xenon_set_gpu_fan_speed(100);
		xenon_set_cpu_fan_speed(100);
	}

	return 0;
}

