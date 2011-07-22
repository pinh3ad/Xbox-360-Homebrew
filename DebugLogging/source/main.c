#include "video_init.h"
#include <stdio.h>

void debug(char *str){
	FILE *DebugLog;
	DebugLog = fopen("uda:/DebugLog.txt","a+");
	fprintf(DebugLog, "%s\n", str);
}

int main(){
	mainInit();

	debug("This will be logged to USB.");
	printf("This will be output to the console and UART.");
}

