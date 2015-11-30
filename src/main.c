/*
 * LINE
 * C project for Buildroot
 * Manage GPIO LED and Buttons
 *
 * Date : Nov 19, 2015
 * Author : Constantin MASSON
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gipy.c"


/**
 * Main function
 */
int main(int argc, char **argv){
	printf("\n\n***** Start *****\n");
	
	int k;
	for(k=0; k<argc; k++){
		printf("argv[%d] = %s\n", k, argv[k]);
	}

	int pin = 18;
	pinValue val = LOGIC_ONE;
	GIPY_pinEnable(pin);
	GIPY_pinWrite(pin, val);
	int val2 = GIPY_pinRead(pin);
	printf("Value : %d\n", val2);

	printf("\n***** End *****\n\n");
	return EXIT_SUCCESS;
}












