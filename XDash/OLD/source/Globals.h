/* Global functions used throughout the program:
\  Reboot to Main Screen, clear the screen.
\
\  Saves time and provides separation.
*/

void clearScreen(){
	int cls;

	for(cls=0;cls<100;cls++){
		printf("\n");
	}
}

void setASCII(){
	printf("##     ## ########     ###     ######  ##     ## \n");
	printf(" ##   ##  ##     ##   ## ##   ##    ## ##     ## \n");
	printf("  ## ##   ##     ##  ##   ##  ##       ##     ## \n");
	printf("   ###    ##     ## ##     ##  ######  ######### \n");
	printf("  ## ##   ##     ## #########       ## ##     ## \n");
	printf(" ##   ##  ##     ## ##     ## ##    ## ##     ## \n");
	printf("##     ## ########  ##     ##  ######  ##     ## \n");
	printf("\n");
}

void reDash(){
	clearScreen();
	setASCII();

	printf("Welcome to XDash!\n");
	printf("Version 0.03\n\n\n");

	printf("Press X to display the hardware temperatures.\n");
	printf("Press B to power down the console.\n");
	printf("Press Y to set the front panel LED color.\n");

	printf("Press the BACK button at any time for main screen.\n");
}

