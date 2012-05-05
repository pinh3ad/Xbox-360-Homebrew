#include "video_init.h"
#include <stdio.h>
#include <stdlib.h>
#include <debug.h>
#include <xenon_smc/xenon_smc.h>

float CPU_TMP = 0, GPU_TMP = 0, MEM_TMP = 0, MOBO_TMP = 0;
uint8_t buf[16];

// Msg the SMC to adjust the system fan speeds. Thanks to Ced2911 :)
void xenon_set_cpu_fan_speed(unsigned val){
	unsigned char msg[16] = { 0x94, (val & 0x7F) | 0x80 };

	xenon_smc_send_message(msg);
}

// Msg the SMC to adjust the system fan speeds. Thanks to Ced2911 :)
void xenon_set_gpu_fan_speed(unsigned val){
	unsigned char msg[16] = { 0x89, (val & 0x7F) | 0x80 };

	xenon_smc_send_message(msg);
}

void getTemps(){
      	memset(buf, 0, 16);
   
      	buf[0] = 0x07;
   
      	xenon_smc_send_message(buf);
      	xenon_smc_receive_response(buf);
   
      	CPU_TMP = (float)((buf[0 * 2 + 1] | (buf[0 * 2 + 2] << 8)) / 256.0);
      	GPU_TMP = (float)((buf[1 * 2 + 1] | (buf[1 * 2 + 2] << 8)) / 256.0);
}

// Regulate the system fan speed based on GPU/CPU temperatures
void regulateTemperatures(){
	// Get the hardware temperatures
	getTemps();	

	// If CPU OR GPU reaches certain temp., change fan states
	if(CPU_TMP == 45 || GPU_TMP == 45){
		xenon_set_cpu_fan_speed(70);
		xenon_set_gpu_fan_speed(70);
		printf("Shifted CPU fan speed to level 1.\n");
		uart_puts("Shifted CPU fan speed to level 1.\n");
	}
	if(CPU_TMP == 53 || GPU_TMP == 53){
		xenon_set_cpu_fan_speed(80);
		xenon_set_gpu_fan_speed(80);
		printf("Shifted CPU fan speed to level 2.\n");
		uart_puts("Shifted CPU fan speed to level 2.\n");
	}
	if(CPU_TMP > 58 || GPU_TMP > 58){
		xenon_set_gpu_fan_speed(100);
		xenon_set_cpu_fan_speed(100);
		printf("Shifted both fans to level 3.\n");
		uart_puts("Shifted both fans to level 3.\n");
	}
}

// TODO: Make this not have to loop, so it doesn't hog cycles
int main(){
	// Init console and video
	videoInit();

	// Loop temperature polling and temperature regulation
	while(1){
		regulateTemperatures();
	}

	return 0;
}

